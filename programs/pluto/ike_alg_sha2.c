/*
 * Copyright (C) 2010-2012 Avesh Agarwal <avagarwa@redhat.com>
 * Copyright (C) 2010-2013 Paul Wouters <paul@redhat.com>
 * Copyright (C) 2013 Florian Weimer <fweimer@redhat.com>
 * Copyright (C) 2013 D. Hugh Redelmeier <hugh@mimosa.com>
 * Copyright (C) 2016 Andrew Cagney <cagney@gnu.org>
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
 *
 */
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <libreswan.h>

#include "constants.h"
#include "lswlog.h"
#include "ike_alg.h"

#include <pk11pub.h>

#include "ike_alg_sha2.h"

/* sha256 thunks */

static void sha256_init_thunk(union hash_ctx *ctx)
{
	sha256_init(&ctx->ctx_sha256);
}

static void sha256_write_thunk(union hash_ctx *ctx, const unsigned char *datap, size_t length)
{
	sha256_write(&ctx->ctx_sha256, datap, length);
}

static void sha256_final_thunk(u_char *hash, union hash_ctx *ctx)
{
	sha256_final(hash, &ctx->ctx_sha256);
}

/* sha384 thunks */

static void sha384_init_thunk(union hash_ctx *ctx)
{
	sha384_init(&ctx->ctx_sha384);
}

static void sha384_write_thunk(union hash_ctx *ctx, const unsigned char *datap, size_t length)
{
	sha384_write(&ctx->ctx_sha384, datap, length);
}

static void sha384_final_thunk(u_char *hash, union hash_ctx *ctx)
{
	sha384_final(hash, &ctx->ctx_sha384);
}

/* sha512 thunks */

static void sha512_init_thunk(union hash_ctx *ctx)
{
	sha512_init(&ctx->ctx_sha512);
}

static void sha512_write_thunk(union hash_ctx *ctx, const unsigned char *datap, size_t length)
{
	sha512_write(&ctx->ctx_sha512, datap, length);
}

static void sha512_final_thunk(u_char *hash, union hash_ctx *ctx)
{
	sha512_final(hash, &ctx->ctx_sha512);
}

struct hash_desc ike_alg_hash_sha2_256 = {
	.common = {
		.name = "sha2_256",
		.officname = "sha256",
		.algo_type = IKE_ALG_HASH,
		.ikev1_oakley_id = OAKLEY_SHA2_256,
		.ikev2_id = IKEv2_PRF_HMAC_SHA2_256,
		.fips = TRUE,
	},
	.hash_ctx_size = sizeof(sha256_context),
	.hash_key_size = SHA2_256_DIGEST_SIZE,
	.hash_digest_len = SHA2_256_DIGEST_SIZE,
	.hash_block_size = 64,	/* from RFC 4868 */
	.hash_init = sha256_init_thunk,
	.hash_update = sha256_write_thunk,
	.hash_final = sha256_final_thunk,
};

struct prf_desc ike_alg_prf_sha2_256 = {
	.common = {
		.name = "sha2_256",
		.officname = "sha256",
		.algo_type = IKE_ALG_PRF,
		.ikev1_oakley_id = OAKLEY_SHA2_256,
		.ikev2_id = IKEv2_PRF_HMAC_SHA2_256,
		.fips = TRUE,
	},
	.prf_key_size = SHA2_256_DIGEST_SIZE,
	.prf_output_size = SHA2_256_DIGEST_SIZE,
	.hasher = &ike_alg_hash_sha2_256,
};

struct integ_desc ike_alg_integ_sha2_256 = {
	.common = {
		.name = "sha2_256",
		.officname = "sha256",
		.algo_type = IKE_ALG_INTEG,
		.ikev1_oakley_id = OAKLEY_SHA2_256,
		.ikev1_esp_id = AUTH_ALGORITHM_HMAC_SHA2_256,
		.ikev2_id = IKEv2_AUTH_HMAC_SHA2_256_128,
		.fips = TRUE,
	},
#if 0
	.hash_ctx_size = sizeof(sha256_context),
	.hash_key_size = SHA2_256_DIGEST_SIZE,
	.hash_digest_len = SHA2_256_DIGEST_SIZE,
	.hash_block_size = 64,	/* from RFC 4868 */
	.hash_init = sha256_init_thunk,
	.hash_update = sha256_write_thunk,
	.hash_final = sha256_final_thunk,
#endif
	.integ_key_size = SHA2_256_DIGEST_SIZE,
	.integ_output_size = SHA2_256_DIGEST_SIZE / 2,
	.prf = &ike_alg_prf_sha2_256,
};

static struct hash_desc ike_alg_hash_sha2_384 = {
	.common = {
		.name = "sha2_384",
		.officname = "sha384",
		.algo_type = IKE_ALG_HASH,
		.ikev1_oakley_id = OAKLEY_SHA2_384,
		.ikev2_id = IKEv2_PRF_HMAC_SHA2_384,
		.fips = TRUE,
	},
	.hash_ctx_size = sizeof(sha384_context),
	.hash_key_size = SHA2_384_DIGEST_SIZE,
	.hash_digest_len = SHA2_384_DIGEST_SIZE,
	.hash_block_size = 128,	/* from RFC 4868 */
	.hash_init = sha384_init_thunk,
	.hash_update = sha384_write_thunk,
	.hash_final = sha384_final_thunk,
};

struct prf_desc ike_alg_prf_sha2_384 = {

	.common = {
		.name = "sha2_384",
		.officname = "sha384",
		.algo_type = IKE_ALG_PRF,
		.ikev1_oakley_id = OAKLEY_SHA2_384,
		.ikev2_id = IKEv2_PRF_HMAC_SHA2_384,
		.fips = TRUE,
	},
	.prf_key_size = SHA2_384_DIGEST_SIZE,
	.prf_output_size = SHA2_384_DIGEST_SIZE,
	.hasher = &ike_alg_hash_sha2_384,
};

struct integ_desc ike_alg_integ_sha2_384 = {
	.common = {
		.name = "sha2_384",
		.officname =  "sha384",
		.algo_type = IKE_ALG_INTEG,
		.ikev1_oakley_id = OAKLEY_SHA2_384,
		.ikev1_esp_id = AUTH_ALGORITHM_HMAC_SHA2_384,
		.ikev2_id = IKEv2_AUTH_HMAC_SHA2_384_192,
		.fips = TRUE,
	},
#if 0
	.hash_ctx_size = sizeof(sha384_context),
	.hash_key_size = SHA2_384_DIGEST_SIZE,
	.hash_digest_len = SHA2_384_DIGEST_SIZE,
	.hash_block_size = 128,	/* from RFC 4868 */
	.hash_init = sha384_init_thunk,
	.hash_update = sha384_write_thunk,
	.hash_final = sha384_final_thunk,
#endif
	.integ_key_size = SHA2_384_DIGEST_SIZE,
	.integ_output_size = SHA2_384_DIGEST_SIZE / 2,
	.prf = &ike_alg_prf_sha2_384,
};

struct hash_desc ike_alg_hash_sha2_512 = {
	.common = {
		.name = "sha2_512",
		.officname = "sha512",
		.algo_type = IKE_ALG_HASH,
		.ikev1_oakley_id = OAKLEY_SHA2_512,
		.ikev2_id = IKEv2_PRF_HMAC_SHA2_512,
		.fips = TRUE,
	},
	.hash_ctx_size = sizeof(sha512_context),
	.hash_key_size = SHA2_512_DIGEST_SIZE,
	.hash_digest_len = SHA2_512_DIGEST_SIZE,
	.hash_block_size = 128,	/* from RFC 4868 */
	.hash_init = sha512_init_thunk,
	.hash_update = sha512_write_thunk,
	.hash_final = sha512_final_thunk,
};

struct prf_desc ike_alg_prf_sha2_512 = {
	.common = {
		.name = "sha2_512",
		.officname = "sha512",
		.algo_type = IKE_ALG_PRF,
		.ikev1_oakley_id = OAKLEY_SHA2_512,
		.ikev2_id = IKEv2_PRF_HMAC_SHA2_512,
		.fips = TRUE,
	},
	.prf_key_size = SHA2_512_DIGEST_SIZE,
	.prf_output_size = SHA2_512_DIGEST_SIZE,
	.hasher = &ike_alg_hash_sha2_512,
};

struct integ_desc ike_alg_integ_sha2_512 = {
	.common = {
		.name = "sha2_512",
		.officname =  "sha512",
		.algo_type = IKE_ALG_INTEG,
		.ikev1_oakley_id = OAKLEY_SHA2_512,
		.ikev1_esp_id = AUTH_ALGORITHM_HMAC_SHA2_512,
		.ikev2_id = IKEv2_AUTH_HMAC_SHA2_512_256,
		.fips = TRUE,
	},
#if 0
	.hash_ctx_size = sizeof(sha512_context),
	.hash_key_size = SHA2_512_DIGEST_SIZE,
	.hash_digest_len = SHA2_512_DIGEST_SIZE,
	.hash_block_size = 128,	/* from RFC 4868 */
	.hash_init = sha512_init_thunk,
	.hash_update = sha512_write_thunk,
	.hash_final = sha512_final_thunk,
#endif
	.integ_key_size = SHA2_512_DIGEST_SIZE,
	.integ_output_size = SHA2_512_DIGEST_SIZE / 2,
	.prf = &ike_alg_prf_sha2_512,
};
