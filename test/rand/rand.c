#include <rand.h>
#include <syscall.h>

int rand(void) {
    return syscall0(SYSCALL_RAND);
}
