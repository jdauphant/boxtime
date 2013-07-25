#!/bin/sh
APPLICATION_NAME=boxtime
BUILD_PATH=dmg
PRIVOXY_BUILD_PATH=privoxy_compile

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

echo "Add proxy service"
cp $WORKING_DIRECTORY/../changelocalproxy_macos/changelocalproxy $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/MacOS/
cp $WORKING_DIRECTORY/../changelocalproxy_macos/clproxy_install.sh $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/MacOS/
#cp $WORKING_DIRECTORY/../$PRIVOXY_BUILD_PATH/privoxy*/privoxy $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/MacOS/
cp $WORKING_DIRECTORY/../privoxy_pkg/MacOS/privoxy $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/MacOS/
mkdir $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/Frameworks
cp $WORKING_DIRECTORY/../privoxy_pkg/Frameworks/* $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/Frameworks 


cd $WORKING_DIRECTORY
echo "Create DMG"
macdeployqt ${APPLICATION_NAME}.app -dmg 

hdiutil convert -format UDRW -o ${APPLICATION_NAME}-rw.dmg ${APPLICATION_NAME}.dmg
mkdir $WORKING_DIRECTORY/mnt
hdiutil mount -mountpoint mnt ${APPLICATION_NAME}-rw.dmg
ln -s /Applications/ mnt/Applications
cp ../${APPLICATION_NAME}/logo.icns mnt/.VolumeIcon.icns
SetFile -a C mnt
hdiutil detach mnt -force 
hdiutil convert -format UDZO -o ${APPLICATION_NAME}-${VERSION}.dmg ${APPLICATION_NAME}-rw.dmg
 
echo "Finish"
