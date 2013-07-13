#!/bin/sh
APPLICATION_NAME=boxtime
VERSION=0.2
BUILD_PATH=deb
DESCRIPTION="Boxtime is a little widget that help you to keep focus on one task."
HOMEPAGE="http://www.boxti.me"
MAINTAINER="Julien DAUPHANT <julien@boxti.me>"
SECTION=gnome

set -x -e

WORKING_DIRECTORY=`pwd`/$BUILD_PATH
ARCHI=amd64
file $WORKING_DIRECTORY/../build-${APPLICATION_NAME}-Desktop-Release/${APPLICATION_NAME} | grep "x86-64" || ARCHI=i386
ROOT_PKG_PATH=$WORKING_DIRECTORY/${APPLICATION_NAME}_${VERSION}_${ARCHI}

echo "Clean working directory"
rm -rf $WORKING_DIRECTORY

echo "Copy packages files"
mkdir -p $ROOT_PKG_PATH/usr/bin
cp build-${APPLICATION_NAME}-Desktop-Release/${APPLICATION_NAME} $ROOT_PKG_PATH/usr/bin
chmod 555 $ROOT_PKG_PATH/usr/bin/${APPLICATION_NAME}

mkdir -p $ROOT_PKG_PATH/usr/share/applications
cat << EOF > $ROOT_PKG_PATH/usr/share/applications/${APPLICATION_NAME}.desktop
[Desktop Entry]
Name=$APPLICATION_NAME
Comment=$HOMEPAGE
Version=$VERSION
Exec=$APPLICATION_NAME
Icon=${APPLICATION_NAME}.png
Terminal=false
Type=Application
Encoding=UTF-8
Categories=GNOME;Utility;Application;
EOF

mkdir -p $ROOT_PKG_PATH/usr/share/pixmaps
cp $WORKING_DIRECTORY/../${APPLICATION_NAME}/ressources/logo_mini.png $ROOT_PKG_PATH/usr/share/pixmaps/${APPLICATION_NAME}.png

echo "Create config files"
cd $ROOT_PKG_PATH
INSTALLED_SIZE=`du --summarize | grep -o "[0-9]*"`
mkdir -p $ROOT_PKG_PATH/DEBIAN
cat << EOF > $ROOT_PKG_PATH/DEBIAN/control
Package: ${APPLICATION_NAME}
Priority: optional
Section: ${SECTION} 
Installed-Size: ${INSTALLED_SIZE}
Maintainer: ${MAINTAINER}
Architecture: ${ARCHI} 
Version: ${VERSION}
Depends: libc6 (>= 2.15), libqtcore4 (>= 4:4.8.0), libqtgui4 (>=4:4.8.0), libqt4-network (>=4:4.8.0)
Description: ${DESCRIPTION}
Homepage: ${HOMEPAGE}
EOF

md5sum `find . -type f | grep -v '^[.]/DEBIAN/'` > $ROOT_PKG_PATH/DEBIAN/md5sums
chmod 644 $ROOT_PKG_PATH/DEBIAN/md5sums

chmod -R g-w $ROOT_PKG_PATH

cd $WORKING_DIRECTORY
fakeroot dpkg-deb -b ${APPLICATION_NAME}_${VERSION}_${ARCHI}
