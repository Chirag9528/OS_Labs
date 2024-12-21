#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){
    int pipefd_a_to_b[2] , pipefd_b_to_a[2];
    int buf;
    if (pipe(pipefd_a_to_b) == -1 || pipe(pipefd_b_to_a) == -1){
        perror("Pipe Failed");
        exit(1);
    }
    int rc = fork();
    if (rc < 0){
        fprintf(stderr , "Fork Failed");
        exit(1);
    }
    else if (rc == 0){
        printf("I am child %d\n",getpid());
        close(pipefd_a_to_b[1]);         // closing writing end of pipe from parent to child
        close(pipefd_b_to_a[0]);        // closing reading end of pipe from child to parent
        int r;
        read(pipefd_a_to_b[0] , &r , sizeof(r));
        printf("%d received %d\n", getpid() , r);
        r+= 10;
        write(pipefd_b_to_a[1] , &r , sizeof(r));
        close(pipefd_a_to_b[0]);
        close(pipefd_b_to_a[1]);
        exit(0);
    }
    else{
        printf("I am parent %d\n", getpid());
        buf = 50;
        close(pipefd_a_to_b[0]);        // closing reading end of pipe from parent to child
        close(pipefd_b_to_a[1]);        // closing writing end of pip from child to parent
        write(pipefd_a_to_b[1] , &buf , sizeof(buf));
        int rc_wait = wait(NULL);
        int new_buf;
        read(pipefd_b_to_a[0] , &new_buf , sizeof(buf));
        printf("%d received %d\n", getpid() , new_buf);
        close(pipefd_a_to_b[1]);
        close(pipefd_b_to_a[0]);
        exit(0);
    }
    return 0;
}