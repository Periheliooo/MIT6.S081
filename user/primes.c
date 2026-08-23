#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
primes(int readfd)
{
    int x, y;
    if (read(readfd, &x, sizeof(int))) {
        int p[2];
        pipe(p);
        
        if (fork() == 0) {
            close(p[1]);
            primes(p[0]);
            close(p[0]);
            exit(0);
        } else {
            close(p[0]);
            printf("prime %d\n", x);
            while (read(readfd, &y, sizeof(int))) {
                if (y % x != 0) {
                    write(p[1], &y, sizeof(int));
                }
            }
            close(p[1]);
            wait(0);
            exit(0);
        }
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    primes(p[0]);
}