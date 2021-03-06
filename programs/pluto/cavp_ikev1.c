/*
 * Parse IKEv1 CAVP test functions, for libreswan
 *
 * Copyright (C) 2015-2016 Andrew Cagney <cagney@gnu.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <pk11pub.h>

#include "lswalloc.h"
#include "ike_alg.h"

#include "ike_alg_sha1.h"
#include "ike_alg_sha2.h"

#include "ikev1_prf.h"
#include "crypt_symkey.h"

#include "cavp.h"
#include "cavp_print.h"
#include "cavp_ikev1.h"

static int ni_length;
static int nr_length;
static int psk_length;
static int g_xy_length;
static struct cavp_entry *prf;

static struct cavp_entry config_entries[] = {
	{ .key = "SHA-1", .op = op_pick, .pick = &prf, .prf = &ike_alg_prf_sha1, },
	{ .key = "SHA-224", .op = op_pick, .pick = &prf, .prf = NULL, },
	{ .key = "SHA-256", .op = op_pick, .pick = &prf, .prf = &ike_alg_prf_sha2_256, },
	{ .key = "SHA-384", .op = op_pick, .pick = &prf, .prf = &ike_alg_prf_sha2_384, },
	{ .key = "SHA-512", .op = op_pick, .pick = &prf, .prf = &ike_alg_prf_sha2_512, },
	{ .key = "Ni length", .op = op_number, .number = &ni_length, },
	{ .key = "Nr length", .op = op_number, .number = &nr_length, },
	{ .key = "pre-shared-key length", .op = op_number, .number = &psk_length, },
	{ .key = "g^xy length", .op = op_number, .number = &g_xy_length, },
	{ .key = NULL }
};

static int count;
static chunk_t psk;
static chunk_t ni;
static chunk_t nr;
static chunk_t cky_i;
static chunk_t cky_r;
static PK11SymKey *g_xy;

static struct cavp_entry data_entries[] = {
	{ .key = "COUNT", .op = op_number, .number = &count },
	{ .key = "g^xy", .op = op_symkey, .symkey = &g_xy },
	{ .key = "Ni", .op = op_chunk, .chunk = &ni },
	{ .key = "Nr", .op = op_chunk, .chunk = &nr },
	{ .key = "CKY_I", .op = op_chunk, .chunk = &cky_i },
	{ .key = "CKY_R", .op = op_chunk, .chunk = &cky_r },
	{ .key = "pre-shared-key", .op = op_chunk, .chunk = &psk },
	{ .key = "SKEYID", .op = op_ignore },
	{ .key = "SKEYID_d", .op = op_ignore },
	{ .key = "SKEYID_a", .op = op_ignore },
	{ .key = "SKEYID_e", .op = op_ignore },
	{ .key = "SKEYID_", .op = op_ignore },
	{ .op = NULL }
};

static void ikev1_skeyid_alphabet(PK11SymKey *skeyid)
{
	PK11SymKey *skeyid_d =
		ikev1_skeyid_d(prf->prf, skeyid,
			       g_xy, cky_i, cky_r);
	print_symkey("SKEYID_d", skeyid_d, 0);

	PK11SymKey *skeyid_a =
		ikev1_skeyid_a(prf->prf, skeyid, skeyid_d,
			       g_xy, cky_i, cky_r);
	print_symkey("SKEYID_a", skeyid_a, 0);

	PK11SymKey *skeyid_e =
		ikev1_skeyid_e(prf->prf, skeyid, skeyid_a,
			       g_xy, cky_i, cky_r);
	print_symkey("SKEYID_e", skeyid_e, 0);

	free_any_symkey("skeyid_d", &skeyid_d);
	free_any_symkey("skeyid_e", &skeyid_e);
	free_any_symkey("skeyid_a", &skeyid_a);
}

static void print_sig_config(void)
{
	config_number("g^xy length", g_xy_length);
	config_key(prf->key);
	config_number("Ni length", ni_length);
	config_number("Nr length", nr_length);
}

static void run_sig(void)
{
	print_number("COUNT", count);
	print_chunk("CKY_I", cky_i, 0);
	print_chunk("CKY_R", cky_r, 0);
	print_chunk("Ni", ni, 0);
	print_chunk("Nr", nr, 0);
	print_symkey("g^xy", g_xy, 0);

	if (prf->prf == NULL) {
		print_line(prf->key);
		return;
	}

	PK11SymKey *skeyid = ikev1_signature_skeyid(prf->prf,
						    ni, nr,
						    g_xy);
	print_symkey("SKEYID", skeyid, 0);
	ikev1_skeyid_alphabet(skeyid);
	free_any_symkey("skeyid", &skeyid);
}

struct cavp cavp_ikev1_sig = {
	.alias = "v1sig",
	.description = "IKE v1 Digital Signature Authentication",
	.print_config = print_sig_config,
	.run = run_sig,
	.config = config_entries,
	.data = data_entries,
};

static void print_psk_config(void)
{
	config_number("g^xy length", g_xy_length);
	config_key(prf->key);
	config_number("Ni length", ni_length);
	config_number("Nr length", nr_length);
	config_number("pre-shared-key length", psk_length);
}

static void run_psk(void)
{
	print_number("COUNT", count);
	print_chunk("CKY_I", cky_i, 0);
	print_chunk("CKY_R", cky_r, 0);
	print_chunk("Ni", ni, 0);
	print_chunk("Nr", nr, 0);
	print_symkey("g^xy", g_xy, 0);
	print_chunk("pre-shared-key", psk, 0);

	if (prf == NULL) {
		print_line(prf->key);
		return;
	}

	PK11SymKey *skeyid = ikev1_pre_shared_key_skeyid(prf->prf, psk,
							 ni, nr);
	print_symkey("SKEYID", skeyid, 0);
	ikev1_skeyid_alphabet(skeyid);
	free_any_symkey("skeyid", &skeyid);
}

struct cavp cavp_ikev1_psk = {
	.alias = "v1psk",
	.description = "IKE v1 Pre-shared Key Authentication",
	.print_config = print_psk_config,
	.run = run_psk,
	.config = config_entries,
	.data = data_entries,
};
