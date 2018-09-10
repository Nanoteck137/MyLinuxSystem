#!/bin/bash

busybox_version=busybox-1.29.3

echo "Downloading busybox" &&
wget https://busybox.net/downloads/"$busybox_version".tar.bz2 &&

echo "Extracting busybox" &&
tar -xvjf "$busybox_version".tar.bz2 &&

mv "$busybox_version" busybox &&
rm "$busybox_version".tar.bz2
