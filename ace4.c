/* ACE 4 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

/* The name of our shell! */
#define SHELLNAME "dora"
/* The number of elements in an array */
#define LEN(array) sizeof(array)/sizeof(array[0])
/* Are two strings equal? */
#define EQ(str1,str2) (!strcmp(str1,str2))
#define UNCHANGED 0
#define MAXIN 512 /* MAXIN is the maximum number of input characters */
#define DELIM " \n\t|><&;" /* DELIM is the string containing all delimiters for tokens */
#define SZ_ARGV 50 /* Size of the argv array */

const char *pathValue;

void quit() {
	setenv("PATH", pathValue, 1);
	printf("%s\n",getenv("PATH"));
	exit(0);
}

char *getcwdir() {
	long size;
	char *buf;
	char *ptr;

	size = pathconf(".", _PC_PATH_MAX);

	/* Potential problem: The allocated memory is never freed. Is
	 * that an issue? (resource leak?) */
	if ((buf = (char *) malloc((size_t) size)) != NULL)
		ptr = getcwd(buf, (size_t) size);

	return ptr;
}

/* Return the PATH environment variable */
void getpath(char **argv) {
	if(argv[1] == NULL)
		printf("%s\n", getenv("PATH"));
	else
		printf("Too many parameters\n");
}

void setpath(char **argv) {
	if(argv[2] == NULL){
		if (argv[1] != NULL) {
			setenv("PATH", argv[1], 1);
		} else {
			printf("Invalid path value: null\n");
		}
	} else 
		printf("Too many parameters\n");
}

char *get_input() {
	static char input[MAXIN]; /* declared as static so it's not on the stack */
	char *cwd = getcwdir();
	bool too_much_input = true;
	int i;

	do {
		printf("[%s]%% ", cwd);

		if (fgets(input, MAXIN, stdin) == NULL){ /* get user input */
			printf("\n");
			quit(); /*Exit on null pointer, given by fgets()*/
		}
	}
	/* fgets as scanf() can't handle blank lines */
	/* check if it was a blank line, i.e. just a '\n' input...*/
	while ('\n' == input[0]); /*check if input is valid - i.e. not blank*/

	/* Clear the rest of the line if it was longer than the input array */
	for (i = 0; i < MAXIN && input[i] != '\0'; i++) {
		switch (input[i]) {
		case '\n': too_much_input = false; break;
		}
	}

	if (too_much_input) {
		while (getchar() != '\n');
	}

	/* If we get to this point it there has to be input so just return it. */
	return (input);
}

void tokenise(char *line, char **tokens) {
	int p;
	char *token;

	p = 0;
	token = strtok(line, DELIM); /* initial strtok call */
	/* While there are more tokens and our array isn't full */
	while (token && (p < SZ_ARGV - 1)) {
		tokens[p++] = token;
		token = strtok(NULL, DELIM); /* ...grab the next token */
	}
	tokens[p] = '\0';
}

int internal_command(char **argv) {
	/* Internal commands */
	/* TODO: internal commands as another function */
	/* exit*/
	if (EQ(argv[0], "exit")) {
		quit();
	} else if (EQ(argv[0], "getpath")) {
		getpath(argv);
		return 0;
	} else if (EQ(argv[0], "setpath")) {
		setpath(argv);
		return 0;
	}

	/* Return negative number if command not found */
	return -1;
}

void external_command(char **argv) {
	pid_t pid;

	/* fork a child process */
	pid = fork();
	if (pid < 0) {
		/* error occurred */
		fprintf(stderr, "Fork Failed");
		return;
	} else if (pid == 0) {
		/* child process */
		execvp(argv[0], argv);
		perror(argv[0]);
		exit(0);
	} else {
		/* parent process */
		/* parent will wait for the child to complete */
		wait(NULL);
		/*printf("Child Complete\n");*/
	}
}

/* Execute looks for the command specified by filename.
 * The order it looks for the command is:
 * 	1. Among built-in commands,
 * 	2. In the PATH.
 *
 * Arguments are passed as an array where the first element
 * is the name of the command we want to run and the following
 * elements are arguments to that command. */
void Execute(char *argv[]) {
	/* Let's make sure there's actually something in the array! */
	if (LEN(argv) == 0) {
		fprintf(stderr,"No arguments given to Execute()");
		return;
	}

	if (internal_command(argv) < 0) {
		external_command(argv);
	}
}

int main() {
	char *input;
	char *argv[SZ_ARGV];

	pathValue = getenv("PATH");
	chdir(getenv("HOME")); /*Changes current working directory to HOME */
	while (1) {
		input = get_input();
		tokenise(input, argv);
		Execute(argv);
	}
	return 0;
}

