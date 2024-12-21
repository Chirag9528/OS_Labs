#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void create_process(int) __attribute__((noreturn));

void create_process(int fd_read){
    int xi;
    read(fd_read, &xi, sizeof(int));
    
    if (xi == -1){
        close(fd_read);
        exit(0);
    }

    printf("%d\n",xi);
    
    int fd1[2];
    while (1){
        int q = pipe(fd1);
        if (q >= 0) break;
    }
    int rc2 = fork();
    if (rc2 < 0){
        fprintf(2 , "Fork SysCall Failed\n");
        close(fd1[0]);
        close(fd1[1]);
        close(fd_read);
        exit(1);
    }
    else if (rc2 == 0){
        close(fd_read);
        close(fd1[1]);
        create_process(fd1[0]);
        exit(0);
    }
    else{
        close(fd1[0]);
        int val;
        read(fd_read , &val , sizeof(int));
        while (val!= -1){
            if (val % xi != 0){
                write(fd1[1] , &val , sizeof(int));
            }
            read(fd_read , &val , sizeof(int));
        }
        write(fd1[1] , &val , sizeof(int));
        close(fd1[1]);
        close(fd_read);
        wait(0);
        exit(0);
    }
}


int main(){
    int n = 269;
    int fd[2];
    int p = pipe(fd);
    if (p == -1){
        fprintf(2 , "Pipe Creation Failed\n");
        exit(1);
    }
    int rc = fork();
    if (rc < 0){
        fprintf(2 , "Fork SysCall Failed\n");
        close(fd[0]);
        close(fd[1]);
        exit(1);
    }
    else if (rc == 0){
        close(fd[1]);
        create_process(fd[0]);
        exit(0);
    }
    else{
        close(fd[0]);
        for (int i=2 ; i<=n ; i++){
            write(fd[1] , &i , sizeof(int));
        }
        int limit = -1;
        write(fd[1] , &limit , sizeof(int));
        close(fd[1]);
        wait(0);
        printf("\n");
    }
    return 0;
}