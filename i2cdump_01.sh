#!/bin/bash
for aa1 in 10 9 8 7
do
    aa2=/sys/class/i2c-adapter/i2c-${aa1}/name
    if `grep -q CH341 ${aa2} 2>/dev/null`
    then
        echo found i2c cable at ${aa1} : ${aa2} 
        grep --color CH341 ${aa2} 
        okok=1
        break
    fi
done

if [ "1" = "${okok}" ] ; then
    #i2cdump -y 8 40
    i2cdump -y ${aa1} 40
else
    echo 'Can NOT find the i2c cable from 10/9/8/7 !'
fi
