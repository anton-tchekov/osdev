#!/bin/bash
avrdude -c arduino -P /dev/ttyACM0 -p m328p -U main.elf
