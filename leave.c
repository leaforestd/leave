#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define MAXBUF 5 /* [+]hhmm */

void unix_error(char* msg);
int Fork(void);
char* Fgets(char* s, int size, FILE* stream);

void main(int argc, char* argv[]) {
	static int loop = 0;
	unsigned int sleep_sec;
	pid_t pid;
	int arg_buf[MAXBUF];
	int index = 0;

	if (argc == 1) { /* no arg */
		int input;

		printf("When do you have to leave?\n");
		while ((input = getchar()) == ' ') {;}
		/* first non-blank character */
		if ((input == '\n') || (input == EOF)) /* blank line, terminate */
			return;
		
		while ((input != '\n') && (input != EOF) && (index < MAXBUF)) {
			/* index: [0, 4] */
			arg_buf[index++] = input;
			input = getchar();
		}
	} else { /* deal with arg */
		int i;
		for (i = 0; i < MAXBUF; i++) {
			arg_buf[i] = argv[1][i] - '0'; /* ASCII -> int */
		}
	}

	if (arg_buf[0] == '+') {
		int j;
		for (j = 1; j < MAXBUF; j++) {
			arg_buf[j] -= '0';
		}

		sleep_sec = (arg_buf[1] * 10 + arg_buf[2]) * 3600 + (arg_buf[3] * 10 + arg_buf[4]) * 60;
		printf("sleep_sec: %d\n", sleep_sec);
	} else {
		if (arg_buf[0]<0 || arg_buf[0]>1 || arg_buf[1]<0 || arg_buf[1]>2 || arg_buf[2]<0 || arg_buf[2]>6 || arg_buf[3]<0 || arg_buf[3]>9) {
			printf("invalid input, exit..\n");
			return;
		}
	}

	
	if ((pid = Fork()) > 0)
		/* terminate your parent, deamon */
		return;

	/* sleep, my lonely child */
	sleep(sleep_sec);
	while (loop <  8) { /* loop 9 times */
		fprintf(stdout, "time to leave!\n");
		sleep(60);
		loop++;
	}	
	/* forced to leave */
	printf("that's the last time I'll tell you. Bye.\n");
	char* execve_argv[] = {"usr/sbin/shutdown", "now", NULL};
	char* execve_environ[] = {NULL};
	execve(execve_argv[0], execve_argv, execve_environ);
}

void unix_error(char* msg) {
	fprintf(stderr, "%s, %s\n", msg, strerror(errno));
	exit(0);
}

int Fork(void) {
	pid_t pid;
	
	if ((pid = fork()) < 0)
		unix_error("fork error");
	return pid;
}

char* Fgets(char* s, int size, FILE* stream) {
	char* ptr;

	if ((ptr = fgets(s, size, stream)) == NULL)
		unix_error("fgets error");
	return ptr;
}

