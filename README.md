# osdev

## Overview

- Modules (source and header files) go in their own directory (example: gfx/)

- `stdlib/` is the standard library for userspace applications.
  We follow the C standard very loosely, but anything useful goes here.

- `platform/` contains drivers for various platforms

- DRIVERS MUST NOT BLOCK THE CPU (exception: during initialization)

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

## Documentation [TODAY]
- All functions documented

## STDLIB [TODAY]
- Make theme easily changable
- GUI Hide/Show elements
- GUI Enable/Disable elements
- Text field copy/cut/paste
- Fix GUI Bugs

## MISC [TODAY]
- Test everything still works

## Text/Code Editor [CUR]
- Line Numbers
- Tab support
- Up/Down Arrow keys + Select
- Syntax highlighting
- Search and Replace
- Goto Line
- Import / Export contents

## Games
- Port Tetris [CUR]
- Port Snake [CUR]

## AVR Platform
- SD Driver write block function [CUR]
- AVR XMEM image routines [CUR]
- PS/2 Keyboard character conversion and Test [CUR]

- Make STDLIB Shared [LATER]
- Async Drivers [LATER]

## Kernel
- Task Scheduler and Memory Protection and Background Tasks [CUR]
- Better SysCall Parameter checking [CUR]
- Exit function [CUR]

- Emulator Single Precision Floating Point Extension + Math Lib [LATER]

## DONE
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
