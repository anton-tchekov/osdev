# AVR Platform Tasks

DONE:
- LCD Basic font [DONE]
- Boot sequence on LCD [DONE]

BORING:
- All functions documented
- Add doxygen

- SD Driver complete and Test
- XMEM Cross Addressing Test

ANNOYING:
- PS/2 Keyboard character conversion and Test

INTERESTING:
- Load binary (App Launcher/Login Screen) from SD card and launch emulator
- Find solution for emulator image routine problem
- Make STDLIB Shared

## DISK LAYOUT / FILE SYSTEM

ATFS = Amazing Technology File System

### Versioning file system
Files cannot be deleted / edited after creation,
each time a file is written, a copy with a incremented version
number is created.
Space allocation is super simple, just increment a pointer.

### Directories are just files containing lists of files

1 block is 512 bytes

START = Start block
SIZE = Size in blocks

START | SIZE | DESCRIPTION
------+------+-----------------------------------------------
    0 |    1 | FS Info (Boot sector)
    1 |  128 | Standard Libary (Shared Libary) (MAX. 64 KiB)
  129 |  128 | Init Program Binary (MAX. 256 KiB)
  257 |    1 | FS Root Directory (Rev. 00)

FS Info Struct

All Integers are little endian

START = Start byte
SIZE = Size in bytes

START | SIZE | DESCRIPTION
------+------+-------------------------------------
    0 |    4 | FS Signature { 'A', 'T', 'F', 'S' }
    4 |    4 | FS Revision (0)
    8 |    4 | STDLIB Size in bytes
   12 |    4 | INIT Program Size in bytes
   16 |    4 | Write Pointer Location
   20 |  492 | Reserved

All files are contiguous

DIR Info struct

Filename separator is dot '.', not slash '/'
Filenames may only contain lowercase letters (a-z), numbers (0-9) and
underscore ('_') and may not start with a number
(basically function identifiers)

File extensions are not supported

Example path: "root.images.vacation.me_on_the_beach"

START | SIZE | DESCRIPTION
------+------+-----------------------------
    0 |    4 | Number of files

File Info struct

START | SIZE | DESCRIPTION
------+------+-----------------------------
    0 |    4 | File Location (start block)
    4 |    8 | File Size (in bytes)
    8 |    N | File Name + null terminator (padding to align at 4 bytes)

Files are sequentially listed with padding to ensure 4 byte align