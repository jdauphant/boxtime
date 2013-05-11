#!/bin/sh
pkill tinyproxy
sleep 1
tinyproxy -c tinyproxy_withblocking.conf
