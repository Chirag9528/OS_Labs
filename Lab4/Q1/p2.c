// wait syscall
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	printf("hello from pid %d\n", getpid());
	int rc = fork();
	if(rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		printf("hello from child %d\n", getpid());
	} else {
		int rc_wait = wait(NULL);
		printf("child %d has completed\n", rc_wait);
		printf("hello from parent %d of child %d\n", getpid(), rc);
	}
}
