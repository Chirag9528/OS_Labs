// kill syscall
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void sighandler_int(int signum) {
	printf("Got INT signal. Exiting now.\n");
	exit(1);
}

void sighandler_term(int signum) {
	printf("Got TERM signal. Exiting now.\n");
	exit(1);
}

int main() {
	signal(SIGINT, sighandler_int);
	signal(SIGTERM, sighandler_term);
	printf("%d\n",getpid());
	while(1) {
		printf("Going to sleep for a second...\n");
		sleep(1);
	}
	return 0;
}
