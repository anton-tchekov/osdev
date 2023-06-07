# osdev

## Overview

- Modules (source and header files) go in their own directory (for example: `gfx/`)

- `apps/` Built-in apps

- `emulator/` contains the kernel and an emulator for a
	RISCV32_IM processor that supports:
	- Base Instructions
	- Multiplication/Division Extension

- `other/` PDFs, Fonts, Misc. Stuff

- `platform/` Drivers for various platforms (Simulator and AVR)

- `shared/` Code that is used by both kernel and userspace

- `stdlib/` is the standard library for userspace applications.
  We follow the C standard very loosely, but anything useful goes here.

- `temp/` Code that was removed but I feel sorry to delete it

- `tools/` Tools for generating fonts, lookup tables, boot images etc.

## Contributing
- Most Important: Simplicity and Responsiveness (eg. No Bloat)
- 80 Characters per line
- Tabs for Indentation
- Spaces for alignment (defines, enums, comments etc.)
- Only multiline comments (`/* comment */`)
- All source files, structs, functions (including static) and global variables
	should be documented using doxygen comments
