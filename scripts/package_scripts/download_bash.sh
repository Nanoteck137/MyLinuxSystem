#!/bin/bash

bash_version=bash-5.0-alpha

echo "Downloading bash" &&
wget https://ftp.gnu.org/gnu/bash/"$bash_version".tar.gz &&

tar xvzf "$bash_version".tar.gz &&

mv "$bash_version" bash &&
rm "$bash_version".tar.gz
