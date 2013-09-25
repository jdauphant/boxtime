#!/bin/sh
ROOT_DIR="$( cd "$( dirname "$0" )" && pwd )"/../

APPLICATION_NAME=boxtime
BUILD_PATH=build/dmg
PRIVOXY_BUILD_PATH=build/privoxy_compile
BIN_PATH=bin/release
PRIVOXY_PATH=build/privoxy_pkg

set -x -e
WORKING_DIRECTORY=$ROOT_DIR/$BUILD_PATH

echo "Retreive QString variables from sourcecode"
eval `cat $ROOT_DIR/${APPLICATION_NAME}/controllers/settingscontroller.h | grep "const QString [A-Z_]* =" | sed -e "s/const QString \([A-Z_]*\) = [a-zA-Z0-9]*(\(.*\))/\1=\2/g"`

APPLICATION_BIN="${ROOT_DIR}/${BIN_PATH}/${APPLICATION_NAME}.app"

echo "Clean working directory"
rm -rf $WORKING_DIRECTORY
mkdir $WORKING_DIRECTORY

echo "Copy package files"
[ -d $APPLICATION_BIN ] ||{ echo "Need Release compiled by QtCreator"; exit; } 
cp -r $APPLICATION_BIN $WORKING_DIRECTORY

echo "Add proxy service"
[ -f ${ROOT_DIR}/${BIN_PATH}/changelocalproxy  ] ||{ 
	echo "Compile changelocalproxy"
	make -C ${ROOT_DIR}/changelocalproxy_macos/ export
	[ -f ${ROOT_DIR}/${BIN_PATH}/changelocalproxy  ] || { echo "Impossible to find changelocalproxy"; exit; } 
}
cp ${ROOT_DIR}/${BIN_PATH}/changelocalproxy $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/MacOS/
cp ${ROOT_DIR}/${BIN_PATH}/clproxy_install.sh $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/MacOS/

cp $ROOT_DIR/$PRIVOXY_PATH/MacOS/privoxy $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/MacOS/
set +e
mkdir $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/Frameworks
set -e
cp $ROOT_DIR/$PRIVOXY_PATH/Frameworks/* $WORKING_DIRECTORY/${APPLICATION_NAME}.app/Contents/Frameworks 


cd $WORKING_DIRECTORY
echo "Create DMG"
macdeployqt ${APPLICATION_NAME}.app -dmg -verbose=2 

hdiutil convert -format UDRW -o ${APPLICATION_NAME}-rw.dmg ${APPLICATION_NAME}.dmg
mkdir $WORKING_DIRECTORY/mnt
hdiutil mount -mountpoint mnt ${APPLICATION_NAME}-rw.dmg
ln -s /Applications/ mnt/Applications
cp $ROOT_DIR/${APPLICATION_NAME}/logo.icns mnt/.VolumeIcon.icns
SetFile -a C mnt
hdiutil detach mnt -force 
hdiutil convert -format UDZO -o ${APPLICATION_NAME}-${VERSION}.dmg ${APPLICATION_NAME}-rw.dmg
 
echo "Finish"
