#!/bin/sh

FS_SAVE=no

#--------TESTING HARDWARE INTERFACES----------
if [ -f /etc/devices/wlan0 ]; then
    if [ `dmesg | grep "NETDEV WATCHDOG: wlan0:" -c` != "0" ]; then
    echo "Wlan 0 interface is no working - SET DOWN FLAG!!!"
    rm -f /etc/devices/wlan0
    echo wlan0 > /etc/devices/bad.wlan0
    FS_SAVE=yes
    fi
fi

if [ -f /etc/devices/eth0 ]; then
    if [ `dmesg | grep "NETDEV WATCHDOG: eth0:" -c` != "0" ]; then
    echo "ETH 0 interface is no working - SET DOWN FLAG!!!"
    rm -f /etc/devices/eth0
    echo eth0 > /etc/devices/bad.eth0
    FS_SAVE=yes
    fi
fi

if [ -f /etc/devices/eth1 ]; then
    if [ `dmesg | grep "NETDEV WATCHDOG: eth1:" -c` != "0" ]; then
    echo "ETH 1 interface is no working - SET DOWN FLAG!!!"
    rm -f /etc/devices/eth1
    echo eth1 > /etc/devices/bad.eth1
    FS_SAVE=yes
    fi
fi

if [ "$FS_SAVE" = "yes" ]; then
    fs save
    reboot
fi
