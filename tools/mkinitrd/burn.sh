#!/bin/bash
if [ $# -eq 0 ]
  then
    echo "Usage: ./burn.sh device"
fi

sudo dd bs=4M if=image.bin of=$1
