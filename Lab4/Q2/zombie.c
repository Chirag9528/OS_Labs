#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    int rc = fork();
    if (rc < 0){
        fprintf(stderr , "fork syscall failed\n");
        exit(1);
    }
    else if (rc == 0){
        printf("Hello i am from child %d\n" , getpid());
        exit(1);
    }
    else{
        printf("Hello I am from parent %d of child %d\n" , getpid() , rc);
    }
    return 0;
}