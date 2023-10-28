#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    char c[1];
    if (fork() == 0) {
        if (read(p[0], c, 1)) {
            int t = getpid();
            fprintf(1, "<%d>: received ping\n", t);
        }
        write(p[1], "1", 1);
        exit(0);
    } else {
        write(p[1], "1", 1);
        if (read(p[0], c, 1)) {
            int t = getpid();
            fprintf(1, "<%d>: received pong\n", t);
        }
    }
    exit(0);
}