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
- Better SysCall Parameter checking
- Cooperative Scheduler, Memory Protection, Background Tasks, Exit function

- Documentation, not again, yes I am forcing myself ...
- GUI Fix Bugs (text width calculation)

- Port Snake Game
- Port Tetris
	- Game title screen
	- Game over screen with high score (new game, back to title, quit)
	- Next piece indicator
	- Score counter (Bonus for multiple lines)
	- Line clear blink
	- Pause Game (Resume, Back to title, New Game, Quit)
	- Better graphics (3D Like effect)

- AVR XMEM image routines
- PS/2 Keyboard character conversion and test
- Fast native memory routines
- SD Driver write block function

## Text/Code Editor features
- Line Numbers
- Tab support
- Up/Down Arrow keys + Select
- Syntax highlighting
- Search and Replace
- Goto Line
- Import / Export contents

## Immediate Next Steps
- File system driver
- App list with icons (load apps from folder)
- File Manager
- Emulate RISC-V Single Precision Floating Point Extension and Math Lib
- Async Drivers

## DONE
- GUI Hide/Show labels (`label_show(&label, true)`)
- Working Login
- GUI Text field copy/cut/paste
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
