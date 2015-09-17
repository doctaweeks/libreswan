ping -w 6 -n -c 1 -I 192.1.3.209 192.1.2.23
# wait on OE retransmits and rekeying
sleep 5
# should show established tunnel and no bare shunts
ipsec whack --trafficstatus
ipsec whack --shuntstatus
ipsec look
# aggressively kill pluto without sending ike delete
killall -9 pluto 
sleep 50
sleep 50
ipsec setup start
/testing/pluto/bin/wait-until-pluto-started
ipsec whack --trafficstatus
ipsec whack --shuntstatus
ping -w 6 -n -c 1 -I 192.1.3.209 192.1.2.23
# ensure for tests acquires expire before our failureshunt=2m
# ping should succeed through tunnel
ping -w 2 -n -c 1 -I 192.1.3.209 192.1.2.23
killall ip > /dev/null 2> /dev/null
cp /tmp/xfrm-monitor.out OUTPUT/road.xfrm-monitor.txt
echo done
