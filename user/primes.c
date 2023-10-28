#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int p[2];
    int vec[35];
    pipe(p);
    for (int i = 2;;i ++) {
        if (fork() == 0) {
            int q, j;
            read(p[0], &q, 4);
            if (q == -1) exit(0);
            fprintf(1, "prime %d\n", q);    
            int cnt = 0;
            while (read(p[0], &j, 4)) {
                if (j == -1) break;
                if (j % q != 0) {
                    vec[cnt] = j;
                    cnt++;
                }
            }
            for (int i = 0; i < cnt; i ++) {
                write(p[1], &vec[i] , 4);
            }
            j = -1;
            write(p[1], &j , 4);
        } else {
            if (i == 2) {
                for (int j = 2; j <= 35; j ++) {
                    write(p[1], &j , 4);
                }
                int j = -1;
                write(p[1], &j , 4);
            }
            wait(0);
            exit(0);
        }
    }
    exit(0);
}