#!/bin/bash

dir=$PWD

echo "Starting to build some packages"

cd ..

rm -rf tmp &&
mkdir tmp &&

cd tmp &&

$dir/package_scripts/download_busybox.sh &&


$dir/package_scripts/download_bash.sh &&

echo "Done building packages"
