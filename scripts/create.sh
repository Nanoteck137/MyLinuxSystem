#!/bin/bash

ROOT=$PWD/../root

echo "Creating the Root filesystem"

sudo rm -rf "$ROOT" &&
sudo mkdir -p "$ROOT"/{etc,tmp,proc,sys,dev,home,mnt,root,usr/{bin,sbin,lib},var} &&
sudo chmod a+rwxt "$ROOT"/tmp &&
sudo ln -s usr/bin "$ROOT/bin" &&
sudo ln -s usr/sbin "$ROOT/sbin" &&
sudo ln -s usr/lib "$ROOT/lib" &&

echo "Done creating the Root filesystem"
