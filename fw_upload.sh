#!/bin/sh

ifconfig eth0 192.168.1.1
tftp -m binary 192.168.1.6 -c put images/wive.bin
ifdown eth0 
#&& ifup eth0