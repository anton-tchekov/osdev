#!/bin/bash
make

cd apps/test/
make
cd ..

cd game/
make
cd ..

cd editor/
make
cd ..

cd tetris/
make
cd ../../

cd platform/avr/
make
cd ../../

cd mkinitrd/
make
cd ..

cd tools/font-convert/
make
cd ..

cd gamma-correction/
make
cd ../..

doxygen
