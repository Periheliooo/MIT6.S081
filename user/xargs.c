#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
    char buf[512];
    char *new_args[MAXARG];
    int num = 0;
    char c;

    for (int i = 1; i < argc; i++) {
        new_args[num] = argv[i];
        num++;
    }

    int i = 0;
    int j = 0;
    while (read(0, &c, 1) > 0) {
        if (c == ' ' || c == '\n') {
            buf[i] = 0;
            i++;
            new_args[num] = &buf[j];
            j = i;
            num++;
        } else {
            buf[i] = c;
            i++;
        }
    }
    new_args[num] = 0;

    if (fork() == 0) {
        exec(new_args[0], new_args);
        exit(0);
    } else {
        wait(0);
        exit(0);
    }
}