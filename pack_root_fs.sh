#!/bin/bash

cd files &&
./copy_files.sh &&
cd .. &&

cd src &&
make &&
sudo cp ./init ../root &&
cd .. &&

cd scripts &&

./pack_root.sh &&

cd .. &&

echo "Done packing the root filesystem"
