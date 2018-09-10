#!/bin/bash

cd src &&
$PWD/build.sh &&

cd .. &&
$PWD/pack_root.sh
