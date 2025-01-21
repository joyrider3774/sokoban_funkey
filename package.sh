#!/bin/sh

make clean
make TARGET=funkey

mkdir -p opk
cp funkoban opk/funkoban
cp -r ./levelpacks opk/levelpacks
cp -r ./graphics opk/graphics
cp -r ./fonts opk/fonts
cp -r ./sound opk/sound
cp -r ./music opk/music
cp Funkoban.funkey-s.desktop opk/Funkoban.funkey-s.desktop

mksquashfs ./opk Funkoban.opk -all-root -noappend -no-exports -no-xattrs

rm -r opk