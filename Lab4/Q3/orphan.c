#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sigterm_handler(){
    printf("I am terminating\n");
    exit(1);
}

int main(){    
    signal(SIGTERM , sigterm_handler);
    int rc = fork();
    if (rc < 0){
        fprintf(stderr , "fork syscall failed");
        exit(1);
    }
    else if (rc == 0){
        int p_id = getppid();
        printf("I am child %d\n", getpid());
        kill(p_id , SIGTERM);
        while (getppid() == p_id){
        }
        printf("parent : %d\n", getppid());
    }
    else{
        // int rc_wait = wait(NULL);
        printf("I am parent %d of child %d\n", getpid() , rc);
        exit(1);
    }
    return 0;
}