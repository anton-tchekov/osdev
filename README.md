# osdev

## TODO:

### stdlib
- Memory allocator
- Print Formatter
- GUI Library
- File read event
- Serial read event
- Keyboard Character<->ASCII Conversion

### Simulator
- Parameter checking to prevent Bugs and Crashes
- Exit function and controlled error handler

### Kernel
- UART Driver
- SPI Driver
- RTC Driver
- LCD Driver
- PS/2 Keyboard Driver
- XMEM Driver

- Task Scheduler
- Memory management

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


