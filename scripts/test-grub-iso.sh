#!/bin/sh
DIR=$(pwd)

mkdir -p iso/boot/grub

cp build/rks.elf iso/boot

cat <<EOF > iso/boot/grub/grub.cfg
set timeout=0
set default=0

menuentry "RKS" {
        multiboot2 /boot/rks.elf
        boot
}
EOF

grub-mkrescue -o iso/rks.iso iso/
