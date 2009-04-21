#!/bin/bash

cd clear &>/dev/null

ip ad add 192.168.1.2/24 dev eth0 &>/dev/null

( echo mode binary; echo put black_fw.bin) | tftp 192.168.1.6

ip ad delete 192.168.1.2/24 dev eth0 &>/dev/null
