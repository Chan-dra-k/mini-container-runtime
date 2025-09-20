#!/bin/bash
set -e
ROOT=rootfs
sudo rm -rf $ROOT
mkdir -p $ROOT/{bin,lib,lib64,usr,proc,sys}
sudo cp /bin/bash $ROOT/bin/
# copy required libs
ldd /bin/bash | awk '/=>/ {print $(NF)}' | xargs -I{} sudo cp -v {} $ROOT/lib64/ 2>/dev/null || true
sudo cp /lib/ld-linux-*.so* $ROOT/lib64/ 2>/dev/null || true
echo "rootfs prepared"
