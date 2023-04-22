#include "fs.h"

Status fs_open(char* name, File *file) {
    return syscall2(SYSCALL_FILE_OPEN, (u32)name, (u32)file);
}
Status fs_read(File file, u32 start, u32 len, void* buf) {
    return syscall4(SYSCALL_FILE_READ, file, start, len, (u32)buf);
}
Status fs_write(File file, u32 start, u32 len, const void* buf) {
    return syscall4(SYSCALL_FILE_WRITE, file, start, len, (u32)buf);
}
Status fs_close(File file) {
    return syscall1(SYSCALL_FILE_CLOSE, file);
}
Status fs_get_size(File file, u32* size) {
    return syscall2(SYSCALL_FILE_SIZE, file, (u32)size);
}