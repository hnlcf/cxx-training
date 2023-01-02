#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void
test_proc_spawn();

int
main(int argc, char **argv)
{
    int8_t  a = -1;
    int32_t b = 20;
    int32_t c = ~((~a + 1) + (~b + 1) - 1);

    printf("%d\n", c);

    return 0;
}

static void
test_proc_spawn()
{

    pid_t pid;
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed.");
            exit(-1);
        } else if (pid == 0) {
            fprintf(stdout, "i=%d, pid=%d, ppid=%d\n", i, getpid(), getppid());
        }
    }

    wait(NULL);
}
