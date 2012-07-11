#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

void unix_error(char* msg);
int Fork(void);
char* Fgets(char* s, int size, FILE* stream);
void invalid_input(void);

void main(int argc, char* argv[]) {
	static int loop = 0;
	unsigned int sleep_sec;
	pid_t pid;
	int arg_buf[5];
	int index = 0;
	int i, j;

	if (argc == 1) { /* no arg */
		int input;

		printf("When do you have to leave?\n");
		while ((input = getchar()) == ' ') {;}
		if ((input == '\n') || (input == EOF)) /* empty line */
			return;
		
		while ((input != '\n') && (input != EOF) && (index < 5)) {
			arg_buf[index++] = input; /* index: [0, 4] */
			input = getchar();
		}

	} else { /* have arg */
		for (i = 0; i < 5; i++) {
			arg_buf[i] = argv[1][i] - '0'; /* ASCII -> int */
		}
	}

	if (arg_buf[0] == '+') { /* +hhmm */
		for (j = 1; j < 5; j++) {
			arg_buf[j] -= '0';
			/* check arg_buf */
			if (arg_buf[j] < 0 || arg_buf[j] > 9)  
				invalid_input();
		}

		sleep_sec = (arg_buf[1] * 10 + arg_buf[2]) * 3600 + (arg_buf[3] * 10 + arg_buf[4]) * 60; /* int -> unsigned, seems no problem? */
		printf("sleep_sec: %d\n", sleep_sec);

	} else { /* hhmm */
		for (j = 0; j < 5 - 1; j++) {
			arg_buf[j] -= '0';
			/* check arag_buf */
			if (arg_buf[j] < 0 || arg_buf[j] > 9) 
				invalid_input();
		}

		if (arg_buf[0]>2 || arg_buf[2]>6) {
			invalid_input();

		time_t time_sec = time(NULL);
		struct tm *time_stc = localtime(&time_sec);
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
	char* execve_argv[] = {"/usr/sbin/shutdown", "now", NULL};
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

void invalid_input(void) {
	printf("leave: usage: leave [+]hhmm\n");
	return;
}
