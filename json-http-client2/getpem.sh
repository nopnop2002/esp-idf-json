#!/bin/bash
#
# Extract the root certificate from www.metaweather.com.

#set -x

HOST="www.howsmyssl.com"

openssl s_client -showcerts -connect ${HOST}:443 </dev/null >hoge
#openssl s_client -showcerts -connect www.howsmyssl.com:443 </dev/null >hoge

start=`grep -e "-----BEGIN CERTIFICATE-----" -n hoge | sed -e 's/:.*//g' | tail -n 1`

last=`grep -e "-----END CERTIFICATE-----" -n hoge | sed -e 's/:.*//g' | tail -n 1`

sed -n ${start},${last}p hoge > main/server_root_cert.pem

rm hoge
