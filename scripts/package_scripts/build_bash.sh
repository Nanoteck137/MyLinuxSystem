#!/bin/bash

echo "Building Bash"

cd bash &&

LDFLAGS=--static ./configure --prefix=$PWD/../../root/ &&
LDFLAGs=--static sudo make install -j6 &&

cd .. &&


echo "Done building Bash"
