#!/bin/bash

cd root
find . | cpio -o -H newc | gzip > ../root.cpio.gz
