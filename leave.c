#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define MAXBUF 128

void unix_error(char* msg);
int Fork(void);
char* Fgets(char* s, int size, FILE* stream);

void main(int argc, char* argv[]) {
	static int loop = 0;
	unsigned int seconds;
	pid_t pid;
	int buf[MAXBUF];
	int* bufptr = buf;

	if (argc == 1) { /* no given arg */
		fprintf(stdout, "When do you have to leave?\n");
		int c = getchar();
		while (c == ' ')
			c = getchar();
		if (c == '\n') /* blank line */
			return;
		
		while (c != '\n') {
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
	char* argv[] = {"shutdown", "now", NULL};
	char* environ[] = {NULL};
	execve(argv[0], argv, environ);
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
	if ((char* ptr = fgets(s, size, stream)) == NULL)
		unix_error("fgets error");

	return ptr;
}

