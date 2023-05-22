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

## Documentation [TODAY]

## STDLIB [CUR]
- Make theme easily changable (`theme_set` function)
- GUI Hide/Show elements (`element_show(&element, true)`)
- GUI Enable/Disable elements (`element_enable(&element, true)`)
- Text field copy/cut/paste
- Fix GUI Bugs (text width calculation)

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

## Init Program
- Login screen
- App list with icons (load apps from folder)

## File Manager
- Planning in progress, no concrete plan on how it should look

## DONE
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
