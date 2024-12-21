// signal syscall
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void sighandler(int signum) {
	printf("Got signal %d\n. Exiting now...\n", signum);
	exit(1);
}

int main() {
	signal(SIGINT, sighandler);
	signal(SIGTSTP , sighandler);
	while(1) {
		printf("Going to sleep for a second...\n");
		sleep(1);
	}
	return 0;
}
