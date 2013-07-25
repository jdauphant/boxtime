#!/bin/sh

PKG_FILE=http://iweb.dl.sourceforge.net/project/ijbswa/Macintosh%20%28OS%20X%29/3.0.21%20%28stable%29/Privoxy%203.0.21%2064%20bit.pkg
BUILD_PATH=privoxy_pkg

set -x -e
WORKING_DIRECTORY=`pwd`/$BUILD_PATH

echo "Clean working directory"
rm -rf $WORKING_DIRECTORY
mkdir $WORKING_DIRECTORY

echo "Retrieve privoxy file"
cd $WORKING_DIRECTORY
curl -O $PKG_FILE

mkdir Frameworks MacOS

xar -xf Privoxy*.pkg
cd pkgContent.pkg
tar -xvf Payload
cp usr/local/sbin/privoxy $WORKING_DIRECTORY"/MacOS"
cp usr/local/lib/*.dylib $WORKING_DIRECTORY"/Frameworks"

cd $WORKING_DIRECTORY

install_name_tool -change /usr/local/lib/libpcre.0.dylib "@loader_path/../Frameworks/libpcre.0.dylib" MacOS/privoxy
install_name_tool -change /usr/local/lib/libpcreposix.0.dylib "@loader_path/../Frameworks/libpcreposix.0.dylib" MacOS/privoxy
install_name_tool -change /usr/local/lib/libpcre.0.dylib "@loader_path/../Frameworks/libpcre.0.dylib" Frameworks/libpcreposix.0.dylib 
