#!/bin/sh

SOURCE_FILE=http://heanet.dl.sourceforge.net/project/ijbswa/Sources/3.0.21%20%28stable%29/privoxy-3.0.21-stable-src.tar.gz
BUILD_PATH=privoxy_compile

set -x -e
WORKING_DIRECTORY=`pwd`/$BUILD_PATH

echo "Clean working directory"
rm -rf $WORKING_DIRECTORY
mkdir $WORKING_DIRECTORY

echo "Retrieve privoxy file"
cd $WORKING_DIRECTORY
curl -O $SOURCE_FILE

tar -xvf privoxy*.tar.gz
cd privoxy*

autoheader
autoconf
./configure
make
