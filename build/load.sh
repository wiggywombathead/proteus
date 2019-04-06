#!/bin/bash
if ! mountpoint -q $HOME/SD; then
    echo "mounting $HOME/SD"
    if ! sudo -E mount /dev/mmcblk0p1 $HOME/SD; then
        echo "mounting failed, exiting"
        exit
    fi
else
    echo "$HOME/SD already mounted"
fi

echo -n "copying to drive... "
sudo -E cp proteus.img $HOME/SD/proteus.img
echo "done"

echo "unmounting $HOME/SD"
sudo -E umount $HOME/SD
