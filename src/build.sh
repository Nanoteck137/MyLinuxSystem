#!/bin/bash

clang++ --static -std=c++14 -Ilua/include/ $PWD/init.cpp lua/liblua.a lua/liblualibs.a -o init &&
cp init ../root
