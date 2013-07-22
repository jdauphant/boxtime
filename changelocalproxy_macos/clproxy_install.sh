#!/bin/sh
PROGRAM=changelocalproxy
INSTALL_DIR=/usr/sbin

set -e

[ -f $PROGRAM ] || { echo "Program $PROGRAM not found"; exit; }
[ -f $INSTALL_DIR"/"$PROGRAM ] && rm $INSTALL_DIR"/"$PROGRAM

cp $PROGRAM $INSTALL_DIR
chown root:wheel $INSTALL_DIR"/"$PROGRAM
chmod +s $INSTALL_DIR"/"$PROGRAM

echo "Install success"
 
