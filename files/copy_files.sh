#!/bin/bash

echo "Copying files"

sudo cp ./group ../root/etc/ &&
sudo chown 0:0 ../root/etc/ &&

sudo cp ./passwd ../root/etc/ &&
sudo chown 0:0 ../root/etc/ &&

sudo cp ./resolv.conf ../root/etc/ &&
sudo chown 0:0 ../root/etc/ &&

sudo cp ./init.conf.lua ../root/etc/ &&
sudo chown 0:0 ../root/etc/ &&

echo "Done copying files"
