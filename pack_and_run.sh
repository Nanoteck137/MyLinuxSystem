#!/bin/bash

./pack_root_fs.sh &&

cd $1 &&

qemu-system-x86_64 -m 512M -nographic -no-reboot -kernel ./linux/arch/x86_64/boot/bzImage -initrd init/root.cpio.gz -append "panic=1 console=ttyS0 mytest=hello"
