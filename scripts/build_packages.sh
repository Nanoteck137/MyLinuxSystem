#!/bin/bash

dir=$PWD

echo "Starting to build some packages"

cd ..

sudo rm -rf tmp &&
mkdir tmp &&

cd tmp &&

$dir/package_scripts/download_busybox.sh &&
$dir/package_scripts/build_busybox.sh &&

$dir/package_scripts/download_bash.sh &&
$dir/package_scripts/build_bash.sh &&

echo "Done building packages"
