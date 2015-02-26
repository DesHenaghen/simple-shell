/* ACE 4 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

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

const char* pathValue;

void quit() {
	setenv("PATH", pathValue, 1);
	exit(0);
}

/* Return the PATH environment variable */
char* getpath() {
	return getenv("PATH");
}

void setpath(char* path) {
	if (path != NULL) {
		setenv("PATH", path, 1);
	} else {
		printf("Invalid path value: null\n");
	}
}

char* get_input(char directory[]) {
	static char input[MAXIN]; /* declared as static so it's not on the stack */
	int c;

	do {
		printf("[%s]%% ",directory);

		if (fgets(input, MAXIN, stdin) == NULL) /* get user input */
			quit(); /*Exit on null pointer, given by fgets()*/
	}
	/* fgets as scanf() can't handle blank lines */
	/* check if it was a blank line, i.e. just a '\n' input...*/
	while ('\n' == input[0]); /*check if input is valid - i.e. not blank*/

	/* Clear the rest of the line if it was longer than the input array */
	int clearinput = 1, i;
	for (i = 0; i < MAXIN; i++) {
		if (input[i] == '\n')
			clearinput = 0;
		if (input[i] == '\0')
			break;
	}
	if (clearinput) {
		while ((c = getchar()) != '\n');
	}
	/* If we get to this point it there has to be input so just return it. */
	return (input);
}

void tokenise(char *line, char **tokens) {
	int p;
	char* token;

	p = 0;
	token = strtok(line, DELIM); /* initial strtok call */
	/* While there are more tokens and our array isn't full */
	while (token && (p < SZ_ARGV - 2)) {
		tokens[p++] = token;
		token = strtok(NULL, DELIM); /* ...grab the next token */
	}
	tokens[p++] = '\0';
}

char* getcwdir() {
	long size;
	char *buf;
	char *ptr;

	size = pathconf(".", _PC_PATH_MAX);

	if ((buf = (char *) malloc((size_t) size)) != NULL)
		ptr = getcwd(buf, (size_t) size);

	return ptr;
}

/* Execute looks for the command specified by filename.
 * The order it looks for the command is:
 * 	1. Among built-in commands,
 * 	2. In the PATH.
 *
 * Arguments are passed as an array where the first element
 * is the name of the command we want to run and the following
 * elements are arguments to that command. 
 *
 * Returns 0 if successful, otherwise returns 1. */
int Execute(char *argv[]) {
	pid_t pid;

	/* Let's make sure there's actually something in the array! */
	if (LEN(argv) == 0) {
		fprintf(stderr, "%s: error: no arguments given to Execute().\n",
				SHELLNAME);
		return (1);
	}

	/* Ensure our file name is properly null-terminated. */
	/* This error checking seems to be doing more harm than good. */
	/*if (filename[LEN(filename)-1] != '\0') {
	 fprintf(stderr,"%s: error: filename is not null terminated.\n",SHELLNAME);
	 return(1);
	 }*/

	/* Internal commands */
	/* TODO: internal commands as another function */
	/* exit*/
	if (EQ(argv[0], "exit")) {
		quit();
	} else if (EQ(argv[0], "getpath")) {
		printf("%s\n", getpath());
		return 0;
	} else if (EQ(argv[0], "setpath")) {
		setpath(argv[1]);
		return 0;
	}

	/* TODO: The rest of the internal commands. */
	/* TODO: Search for command in path. */

	/* fork a child process */
	pid = fork();
	if (pid < 0) {
		/* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	} else if (pid == 0) {
		/* child process */

		execvp(argv[0], argv);
		perror(argv[0]);
		exit(0);
	} else {
		/* parent process */
		/* parent will wait for the child to complete */
		wait(NULL);
		printf("Child Complete \n");
	}
}

int main() {
	pathValue = getpath();
	chdir(getenv("HOME")); /*Changes current working directory to HOME */
	char *directory;
	char *input;
	char *argv[SZ_ARGV];
	while (1) {
		directory = getcwdir(); /*gets current working directory*/
		input = get_input(directory);
		tokenise(input, argv);
		Execute(argv);
	}
	return 0;
}

