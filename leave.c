#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

typedef struct hm {
	int h;
	int m;
} hm;

void unix_error(char* msg);
int Fork(void);
char* Fgets(char* s, int size, FILE* stream);
void invalid_input(void);
void count_sleep_hm(hm* now, hm* shutdown, hm* sleep);

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
			arg_buf[index++] = input;
			input = getchar();
		}

	} else { /* have arg */
		for (i = 0; i < 5; i++) {
			arg_buf[i] = argv[1][i];
		}
	}

	if (arg_buf[0] == '+') { /* +hhmm */
		for (j = 1; j < 5; j++) {
			arg_buf[j] -= '0'; /* ASCII -> int */
			/* check arg_buf */
			if (arg_buf[j] < 0 || arg_buf[j] > 9)  
				invalid_input();
		}

		sleep_sec = (arg_buf[1] * 10 + arg_buf[2]) * 3600 + (arg_buf[3] * 10 + arg_buf[4]) * 60; /* int -> unsigned, seems no problem? */
	} else { /* hhmm */
		for (j = 0; j < 5 - 1; j++) {
			arg_buf[j] -= '0'; /* ASCII -> int */
			/* check arag_buf */
			if (arg_buf[j] < 0 || arg_buf[j] > 9) 
				invalid_input();
		}

		if (arg_buf[0]>2 || arg_buf[2]>6) 
			invalid_input();
		if ((arg_buf[0] == 2) && (arg_buf[1] > 3))
			invalid_input();

		time_t sec = time(NULL);
		struct tm *now_ptr = localtime(&sec);

		hm* now = (hm*) malloc(sizeof(hm));
		hm* shutdown = (hm*) malloc(sizeof(hm));
		hm* sleep = (hm*) malloc(sizeof(hm));

		now->h = now_ptr->tm_hour;
		now->m = now_ptr->tm_min;
		shutdown->h = arg_buf[0] * 10 + arg_buf[1];
		shutdown->m = arg_buf[2] * 10 + arg_buf[3];

		count_sleep_hm(now, shutdown, sleep);
		sleep_sec = sleep->h * 3600 + sleep->m * 60;

		free(now);
		free(shutdown);
		free(sleep);
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
	printf("leave: invalid input\nusage: leave [+]hhmm\n");
	exit(0);
}

void count_sleep_hm(hm* now, hm* shutdown, hm* sleep) {
	sleep->h = shutdown->h - now->h;
	if (sleep->h < 0)
		sleep->h += 24;

	sleep->m = shutdown->m - now->m;
	if (sleep->m < 0) {
		sleep->m += 60;
		sleep->h--;
	}
}
