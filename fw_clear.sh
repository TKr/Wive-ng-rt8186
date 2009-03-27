#!/bin/sh

ifconfig eth0 192.168.1.1
tftp -m binary 192.168.1.6 -c put clear/black_fw.bin
