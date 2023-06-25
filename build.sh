#!/bin/bash
clear

echo -ne "\n\n\n>>> BUILDING APPS/INIT/\n\n\n"

cd apps/init/
make
cd ../../

echo -ne "\n\n\n>>> BUILDING APPS/EDITOR/\n\n\n"

cd apps/editor/
make
cd ../../

echo -ne "\n\n\n>>> BUILDING APPS/PERF/\n\n\n"

cd apps/perf/
make
cd ../../

echo -ne "\n\n\n>>> BUILDING APPS/TETRIS/\n\n\n"

cd apps/tetris/
make
cd ../../

echo -ne "\n\n\n>>> BUILDING APPS/SNAKE/\n\n\n"

cd apps/snake/
make
cd ../../

echo -ne "\n\n\n>>> BUILDING PLATFORM/SIMULATOR/\n\n\n"

cd platform/simulator/
make clean && make
cd ../../

echo -ne "\n\n\n>>> BUILDING PLATFORM/AVR/\n\n\n"

cd platform/avr/
make clean && make
cd ../../

echo -ne "\n\n\n>>> BUILDING PLATFORM/X86/\n\n\n"

cd platform/x86/
make clean && make
cd ../../

echo -ne "\n\n\n>>> BUILDING TOOLS/FONT_CONVERT/\n\n\n"

cd tools/font-convert/
make
cd ../../

echo -ne "\n\n\n>>> BUILDING TOOLS/MKINITRD/\n\n\n"

cd tools/mkinitrd/
make
cd ../../

echo -ne "\n\n\n>>> BUILDING TOOLS/GAMMA_CORRECTION/\n\n\n"

cd tools/gamma-correction/
make
cd ../../
