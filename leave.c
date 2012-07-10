#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define MAXBUF 5 /* [+]hhmm */

void unix_error(char* msg);
int Fork(void);
char* Fgets(char* s, int size, FILE* stream);

void main(int argc, char* argv[]) {
	static int loop = 0;
	unsigned int seconds;
	pid_t pid;
	char arg_buf[MAXBUF];
	int index = 0;

	if (argc == 1) { /* no given arg */
		int input;

		fprintf(stdout, "When do you have to leave?\n");
		while ((input = getchar()) == ' ') {;}
		/* first non-blank character */
		if ((input == '\n') || (input == EOF)) /* blank line, terminate */
			return;
		
		while ((input != '\n') && (input != EOF)) {
			if (index < MAXBUF)
				arg_buf[index++] = input; /* int -> char */
			input = getchar();
		}
	} else {
		int i;
		for (i = 0; i < MAXBUF; i++) {
			arg_buf[i] = argv[1][i];
		}
	}
	
	if ((pid = Fork()) > 0)
		/* terminate parent */
		return;

	/* sleep, my lonely child */
	sleep(seconds);
	while (loop <  8) { /* loop 9 times */
		fprintf(stdout, "time to leave!\n");
		sleep(60);
		loop++;
	}	
	/* forced to leave */
	fprintf(stdout, "that's the last time I'll tell you. Bye.\n");
	char* execve_argv[] = {"shutdown", "now", NULL};
	char* execve_environ[] = {NULL};
	execve(argv[0], execve_argv, execve_environ);
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

