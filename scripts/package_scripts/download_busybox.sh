#!/bin/bash

echo "Downloading busybox" &&
wget https://busybox.net/downloads/busybox-1.29.3.tar.bz2 &&

echo "Extracting busybox" &&
tar -xvjf busybox-1.29.3.tar.bz2
