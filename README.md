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

# TODO
- FS:     File system driver, File Manager, Terminal
- INIT:   App list with icons (load apps from folder)

# TODO

## Snake

## Tetris
- Game title screen
- Game over screen with high score (new game, back to title, quit)
- Next piece indicator
- Score counter (Bonus for multiple lines)
- Line clear blink
- Pause Game (Resume, Back to title, New Game, Quit)
- Better graphics (3D Like effect)

## Text/Code Editor features
- Line Numbers
- Tab support
- Up/Down Arrow keys + Select
- Syntax highlighting
- Search and Replace
- Goto Line
- Import / Export contents

# Future
- Scheduler, Background Tasks
- Fast native memory routines
- String syscall (default font) as optimization
- Emulate RISC-V Single Precision Floating Point Extension and Math Lib
- Async Drivers

# DONE
- AVR SD Driver write block function
- SysCall Parameter checking, Memory Protection
- Exit function
- Documentation Todos
- GUI Fix text width calculation bugs
- Invert labels
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
