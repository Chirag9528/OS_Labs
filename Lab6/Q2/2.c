#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void create_process(int n , int fd_read){
    int xi;
    if (read(fd_read, &xi, sizeof(int)) <= 0) {
        close(fd_read);
        exit(0);
    }
    printf("%d\n",xi);
    if (xi >= n) {
        close(fd_read);
        exit(0);
    }

    int fd1[2];
    int q = pipe(fd1);
    if (q == -1){
        fprintf(stderr , "Pipe Creation Failed\n");
        exit(1);
    }
    int rc2 = fork();
    if (rc2 < 0){
        fprintf(stderr , "Fork SysCall Failed\n");
        exit(1);
    }
    else if (rc2 == 0){
        close(fd1[1]);
        create_process(n , fd1[0]);
        exit(0);
    }
    else{
        close(fd1[0]);
        int val;
        int temp = read(fd_read , &val , sizeof(int));
        while (temp > 0){
            if (val % xi != 0){
                write(fd1[1] , &val , sizeof(int));
            }
            temp = read(fd_read , &val , sizeof(int));
        }
        close(fd1[1]);
        close(fd_read);
        wait(NULL);
        exit(0);
    }
}


int main(){
    int n = 280;
    int fd[2];
    int p = pipe(fd);
    if (p == -1){
        fprintf(stderr , "Pipe Creation Failed\n");
        exit(1);
    }
    
    int rc = fork();
    if (rc < 0){
        fprintf(stderr , "Fork SysCall Failed\n");
        exit(1);
    }
    else if (rc == 0){
        close(fd[1]);
        create_process(n , fd[0]);
        exit(0);
    }
    else{
        close(fd[0]);
        for (int i=2 ; i<=n ; i++){
            write(fd[1] , &i , sizeof(int));
        }
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}