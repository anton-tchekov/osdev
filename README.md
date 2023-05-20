# osdev

## TODO:

### stdlib
- GUI Library [PRIO]
	- Init function refactoring [DONE]
	- Replace current theme with Dark Mode []
		- Make theme easily changable []

	- Add utility functions
		- Hide / Show elements

	- Text field
		- Changable Font(Size)
		- Multiline(Select)
		- Line Numbers
		- Tab support
		- Up/Down Arrow keys + Shift
		- Left/Right/Center Align
		- Syntax highlighting
		- Copy paste
		- Unicode support
		- Search and Replace
		- Goto Line
		- Import / Export contents

- Memory allocator [DONE]
- Keyboard Character<->ASCII Conversion [DONE]

- Unicode []
	- Text Field []
	- Keyboard [DONE]

### Simulator
- Parameter checking to prevent Bugs and Crashes [TODO]
- Exit function and controlled error handler [DONE]

### Kernel
- Task Scheduler [PRIO]
- Paging [PRIO]

## Contributing
- 80 Characters per line
- Tabs for Indentation
- Spaces for alignment (defines, enums, comments etc.)
- All source files, structs, functions and global variables should be
  documented using doxygen comments

- Only multiline comments (`/* comment */`)

## Overview

- Modules (source and header files) go in their own directory (example: gfx/)

- `stdlib/` is the standard library for userspace applications.
  We follow the C standard very loosely, but anything useful goes here.

- `platform/` contains drivers for various platforms

- DRIVERS MUST NOT BLOCK THE CPU (exception: during initialization)

- `emulator/` contains a RISC-V emulator

Emulates a RISCV32_IM processor.

Supports:
- Base Instructions
- Multiplication/Division Extension

Work in Progress:
- Single Precision Floating Point Extension

- `apps` built-in apps
