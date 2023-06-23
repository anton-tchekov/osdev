# Disk Driver

This folder contains the disk driver. It is the worst possible way a disk
driver can be implemented but also the easiest (This is not an understatement).
It works by dropping back into real mode and then using BIOS interrupts to
read and write sectors from disk.

Why is this bad?
- Uses Legacy BIOS functions that may be removed in the future
- It can freeze the Computer indefinately
- PC might have UEFI and no BIOS
- It is extremely slow and does not support asynchronous I/O
