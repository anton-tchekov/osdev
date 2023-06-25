# Operating System Development Experiment

## Overview

- Modules (source and header files) go in their own directory
	(for example: `gfx/`)

- `apps/` Built-in apps

- `emulator/` contains the kernel and an emulator for a
	RISCV32_IM processor that supports:
	- Base Instructions
	- Multiplication/Division Extension

- `other/` PDFs, Fonts, Old code backups, Misc. Stuff

- `platform/` Drivers for various platforms (Simulator and AVR)

- `platform/x86/` x86 OS Subproject

- `shared/` Code that is used by both kernel and userspace

- `stdlib/` is the standard library for userspace applications.
  We follow the C standard very loosely, anything useful goes here.

- `tools/` Tools for generating fonts, lookup tables, boot images etc.

- `build.sh` Script that compiles (almost) everything

- `Doxyfile` Doxygen Configuration File

- `TODO.md` TODO List

## Contributing
- Most Important: Simplicity and Responsiveness (eg. No Bloat)
- 80 Characters per line
- Tabs for Indentation
- Spaces for alignment (defines, enums, comments etc.)
- Only multiline comments (`/* comment */`)
- All source files, structs, functions (including static) and global variables
	should be documented using doxygen comments (except x86 subproject)
