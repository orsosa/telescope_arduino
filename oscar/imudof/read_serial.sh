#!/bin/bash
#tty=`dmesg | tail -1 | awk '{print$12}'`
tty=`ls /dev/ttyUSB?`
stty -F $tty cs8 57600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
cat < $tty
