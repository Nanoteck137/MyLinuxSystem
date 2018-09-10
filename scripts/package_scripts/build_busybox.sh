#!/bin/bash

echo "Building Busybox"

cd busybox &&

make defconfig &&
sudo LDFLAGS=--static make CONFIG_PREFIX=../../root install -j6
    
cd .. &&

echo "Done building Busybox"
