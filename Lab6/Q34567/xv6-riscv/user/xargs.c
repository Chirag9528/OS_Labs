#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{  
    if (argc < 2){
        fprintf(2 , "Xargs Syscall failed\n");
        exit(1);
    }
    char c;
    while (1){
        if (read(0 , &c , 1) <= 0) break;
        char cmd[MAXARG];
        cmd[0] = c;
        int i = 1;
        while (1){
            read(0 , &cmd[i] , sizeof(char));
            if (cmd[i] == '\n') break;
            i++;
        }
        int rc = fork();
        if (rc < 0){
            fprintf(2,"Some Error: Fork Failed\n");
            exit(1);
        }
        else if (rc == 0){
            char* arg[MAXARG];
            arg[0] = argv[1];
            int j = 2;
            while (j < argc){
                arg[j-1] = argv[j];
                j++;
            }
            arg[j-1] = cmd;
            arg[j] = 0;

            exec(arg[0] , arg);
        }
        else{
            wait(0);
        }
    }
    exit(0);
}