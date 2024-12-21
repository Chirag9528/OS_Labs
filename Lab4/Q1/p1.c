// Fork Syscall
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	printf("hello from pid %d\n", getpid());
	int rc = fork();
	if(rc < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		printf("hello from child %d\n", getpid());
	} else {
		printf("hello from parent %d of child %d\n", getpid(), rc);
	}
}
