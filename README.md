# osdev

## TODO:

### stdlib
- GUI Library [PRIO]
	- Changable Font(Size) for elements
	- Init function refactoring [DONE]
	- Multiline(Select) Text field
	- Line Number Text field
	- Tab in Text field
	- Syntax highlighting in text field
	- Copy paste text field

- Memory allocator [DONE]
- Keyboard Character<->ASCII Conversion [DONE]

### Simulator
- Parameter checking to prevent Bugs and Crashes [TODO]
- Exit function and controlled error handler [DONE]

### Kernel
- UART Driver [PRIO]
- SPI Driver [PRIO]
- XMEM Driver [PRIO]
- LCD Driver [PRIO]
- PS/2 Keyboard Driver [PRIO]

- SD Driver [TODO]
- RTC Driver [TODO]

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

- DRIVERS MUST NOT BLOCK THE CPU (exception: initialization)

- `emulator/` contains a RISC-V emulator

Emulates a RISCV32_IM processor.

Supports:
- Base Instructions
- Multiplication/Division Extension

Work in Progress:
- Single Precision Floating Point Extension

- `apps` built-in apps


