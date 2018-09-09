#!/bin/bash

ROOT=$PWD/root

rm -rf "$ROOT" &&
mkdir -p "$ROOT"/{etc,tmp,proc,sys,dev,home,mnt,root,usr/{bin,sbin,lib},var} &&
chmod a+rwxt "$ROOT"/tmp &&
ln -s usr/bin "$ROOT/bin" &&
ln -s usr/sbin "$ROOT/sbin" &&
ln -s usr/lib "$ROOT/lib" &&

cat > "$ROOT"/etc/passwd << 'EOF' &&
root::0:0:root:/root:/bin/bash
guest:x:500:500:guest:/home/guest:/bin/bash
nobody:x:65534:65534:nobody:/proc/self:/dev/null
EOF

cat > "$ROOT"/etc/group << 'EOF' &&
root:x:0:
guest:x:500:
EOF

echo "nameserver 8.8.8.8" > "$ROOT"/etc/resolv.conf || exit 1    

