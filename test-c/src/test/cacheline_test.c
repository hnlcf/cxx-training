#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include <test/cacheline_test.h>

static const size_t kBufSize = 1024;

void
test_cacheline_atomic(int offset)
{
    int pid, status;
    int count = 80;

    int write_count = 10000000;
    int read_count = 30000000;

    char* buf;
    unsigned long data;

    int i, j;

    buf = mmap(NULL,
               kBufSize,
               PROT_READ | PROT_WRITE,
               MAP_SHARED | MAP_ANONYMOUS,
               -1,
               0);
    memset(buf, 0xdd, kBufSize);
    buf = (void*)((unsigned long)(buf + 63) & 0xffffffffffffffc0);

    for (i = count; i; i--) {
        // spawn a child process
        pid = fork();
        if (pid == 0) {
            // assign different value to write cacheline for different child
            // process
            int child_pid = getpid();
            if (child_pid % 2) {
                data = 0xffffffffffffffff;
            } else {
                data = 0x0000000000000000;
            }

            // write repeatedly
            for (j = write_count; j; j--) {
                // disable optimization by using `volatile`
                *(volatile unsigned long*)(buf + offset) = data;
            }

            exit(0);
        } else {
            // parent
        }
    }

    for (j = read_count; j; j--) {
        data = *(unsigned long*)(buf + offset);

        // non-atomic when write cross cacheline
        if (data != 0xffffffffffffffff && data != 0) {
            printf("non-atomic: %016lx\n", data);
        }
    }

    while ((pid = wait(&status)) > 0)
        ;
}