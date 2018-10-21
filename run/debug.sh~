#!/bin/sh 

ifconfig wlan0 192.168.3.1
hostapd -d /etc/hostapd.conf -B
udhcpd /etc/udhcpd.conf

ifconfig eth0 192.168.10.3 netmask 255.255.255.0
mount -t nfs -o nolock 192.168.10.4:/home/tl/shareSAMBA/MINILOC/OMAPL138 /nfs
cd /nfs/run
insmod syslink.ko
./slaveloader startup DSP ../dsp/Debug/MINILOC.out
gdbserver 192.168.10.4:10000 ./../host/Debug/MINILOC

