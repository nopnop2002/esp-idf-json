#!/bin/bash
#
# Extract the root certificate from www.metaweather.com.

#set -x

#openssl s_client -showcerts -connect www.metaweather.com:443 </dev/null >hoge
openssl s_client -showcerts -connect weatherdbi.herokuapp.com:443 </dev/null >hoge

start=`grep -e "-----BEGIN CERTIFICATE-----" -n hoge | sed -e 's/:.*//g' | tail -n 1`

last=`grep -e "-----END CERTIFICATE-----" -n hoge | sed -e 's/:.*//g' | tail -n 1`

sed -n ${start},${last}p hoge > main/metaweather_com_root_cert.pem

rm hoge
