# osdev

## Overview

- Modules (source and header files) go in their own directory (example: gfx/)

- `stdlib/` is the standard library for userspace applications.
  We follow the C standard very loosely, but anything useful goes here.

- `platform/` contains drivers for various platforms

- `emulator/` contains an emulator for a RISCV32_IM processor that supports:
	- Base Instructions
	- Multiplication/Division Extension

- `apps` built-in apps

## Contributing
- Most Important: Simplicity and Responsiveness (eg. No Bloat)
- 80 Characters per line
- Tabs for Indentation
- Spaces for alignment (defines, enums, comments etc.)
- Only multiline comments (`/* comment */`)
- All source files, structs, functions (including static) and global variables
	should be documented using doxygen comments

# Global TODO List

## CURRENT in order of priority
- GUI Text field copy/cut/paste
- GUI Fix Bugs (text width calculation)
- Better SysCall Parameter checking

- Port Tetris
	- Game title screen
	- Game over screen with high score
	- Next piece indicator
	- Score counter
	- Line clear blink
	- Pause Game

- AVR XMEM image routines
- PS/2 Keyboard character conversion and test

## Init
- Working Login
- App list with icons (load apps from folder)

## Text/Code Editor features
- Line Numbers
- Tab support
- Up/Down Arrow keys + Select
- Syntax highlighting
- Search and Replace
- Goto Line
- Import / Export contents

## AVR Platform in order of priority
- Fast native memory routines
- SD Driver write block function

## Kernel in order of importance
- Cooperative Scheduler, Memory Protection, Background Tasks, Exit function
- File system driver

## FOR LATER in order of difficulty
- File Manager
- Emulate RISC-V Single Precision Floating Point Extension and Math Lib
- Async Drivers

## Ideas
- Port Snake Game
- GUI Hide/Show elements (`element_show(&element, true)`)
- GUI Enable/Disable elements (`element_enable(&element, true)`)

## DONE
- GUI Make theme easily changable (`theme_set` function)
- Login screen
- Build everything script
- All functions documented
- Dark Mode Theme
- Add doxygen
- Memory allocator
- Emulator German Keyboard Layout<->ASCII Conversion
- AVR LCD Basic Font
- Boot Sequence on LCD
- XMEM Cross-Bank Addressing Test
- Load first binary from SD card and launch emulator
- GUI Element Init function refactoring
- Password field stars option
