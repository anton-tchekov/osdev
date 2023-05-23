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

## TODAY in order of priority
- GUI Make theme easily changable (`theme_set` function)
- GUI Text field copy/cut/paste

- Port Tetris
	- Game title screen
	- Game over screen with high score
	- Next piece indicator
	- Score counter
	- Line clear blink

## STDLIB
- GUI Fix Bugs (text width calculation)
- GUI Hide/Show elements (`element_show(&element, true)`)
- GUI Enable/Disable elements (`element_enable(&element, true)`)

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
- AVR XMEM image routines
- PS/2 Keyboard character conversion and test
- Fast native memory routines
- SD Driver write block function

## Kernel in order of importance
- Better SysCall Parameter checking
- Cooperative Scheduler, Memory Protection, Background Tasks, Exit function

## FOR LATER in order of difficulty
- Port Snake Game
- File Manager
- Make STDLIB Shared
- Emulate RISC-V Single Precision Floating Point Extension and Math Lib
- Async Drivers
- Full preemptive scheduler and paging



## DONE
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
