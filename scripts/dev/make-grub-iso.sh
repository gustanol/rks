#!/bin/sh
DIR=$(pwd)

rm -rf iso
mkdir -p iso/boot/grub

cp build/rks.bin iso/boot

cat <<EOF > iso/boot/grub/grub.cfg
set timeout=0
set default=0

menuentry "RKS" {
        multiboot2 /boot/rks.bin
        boot
}
EOF

grub-mkrescue -o iso/rks.iso iso/
