#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    char* args[MAXARG]; 
    char c;
    int num_args = 0;
    int len_arg = 0;
    for (int i = 1; i < argc; i ++) {
        args[num_args++] = argv[i];
    }
    args[num_args] = malloc(1024);
    int num_args_start = num_args;
    while (read(0, &c, 1) > 0) {
        if (c == ' ') {
            args[num_args][len_arg] = 0;
            num_args ++;
            len_arg = 0;
            args[num_args] = malloc(1024);
        } else if (c == '\n') {
            args[num_args][len_arg] = 0;
            num_args ++;
            len_arg = 0;
            args[num_args] = 0;
            if (fork() == 0) {
                exec(argv[1], args);
            } else {
                wait(0);
            }
            for (int i = num_args_start; i < num_args; i ++) {
                free(args[i]);
            }
            num_args = num_args_start;
            len_arg = 0;
            args[num_args_start] = malloc(1024);
        } else {
            args[num_args][len_arg++] = c;
        }
    }
    return 0;
}