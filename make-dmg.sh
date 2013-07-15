#!/bin/sh
APPLICATION_NAME=boxtime
BUILD_PATH=dmg

set -x -e
WORKING_DIRECTORY=`pwd`/$BUILD_PATH

echo "Retreive QString variables from sourcecode"
eval `cat $WORKING_DIRECTORY/../${APPLICATION_NAME}/controllers/settingscontroller.h | grep "const QString [A-Z_]* =" | sed -e "s/const QString \([A-Z_]*\) = [a-zA-Z0-9]*(\(.*\))/\1=\2/g"`

echo "Clean working directory"
rm -rf $WORKING_DIRECTORY
mkdir $WORKING_DIRECTORY

echo "Copy package files"
[ -d $WORKING_DIRECTORY/../build-${APPLICATION_NAME}-Desktop-Release/${APPLICATION_NAME}.app/ ] ||{ echo "Need Release compiled by QtCreator"; exit; } 
cp -r $WORKING_DIRECTORY/../build-${APPLICATION_NAME}-Desktop-Release/${APPLICATION_NAME}.app $WORKING_DIRECTORY

echo "Create DMG"
macdeployqt $WORKING_DIRECTORY/${APPLICATION_NAME}.app -dmg 

hdiutil convert -format UDRW -o $WORKING_DIRECTORY/${APPLICATION_NAME}-rw.dmg $WORKING_DIRECTORY/${APPLICATION_NAME}.dmg
mkdir $WORKING_DIRECTORY/mnt
hdiutil mount -mountpoint $WORKING_DIRECTORY/mnt $WORKING_DIRECTORY/${APPLICATION_NAME}-rw.dmg
ln -s /Applications/ $WORKING_DIRECTORY/mnt/Applications
hdiutil detach $WORKING_DIRECTORY/mnt -force 
rm $WORKING_DIRECTORY/${APPLICATION_NAME}.dmg
hdiutil convert -format UDZO -o $WORKING_DIRECTORY/${APPLICATION_NAME}-${VERSION}.dmg $WORKING_DIRECTORY/${APPLICATION_NAME}-rw.dmg
 
echo "Finish"
