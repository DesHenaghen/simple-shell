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

/*A structure for storing the command number and string*/
typedef struct {
  int cmd_no;
  char input_line[MAXIN];
  
} history_line_t;


/* An array for storing history*/ 
static history_line_t saved_history [20]; 

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

void cd(char *directory) {
  if (!directory)
    directory = getenv("HOME");

  if (chdir(directory))
    perror(directory);
}

/* Return the PATH environment variable */
void getpath(char **argv) {
	if(argv[1] == NULL)
		printf("%s\n", getenv("PATH"));
	else
		printf("Too many parameters\n");
}

void setpath(char **argv) {
	if(argv[2] == NULL) {
		if (argv[1] != NULL) {
			setenv("PATH", argv[1], 1);
		} else {
			printf("Invalid path value: null\n");
		}
	}
}

/* 
This instruction is called when the first character of the input is a '!'
meaning a command is invoked from history
*/

char *command_history(char *input, int count) {
	int cmd;
/* '-' means counting backwards from the last commands entered */ 
	if ('-' == input[1]) { 
/* input +2 makes it that a pointer to the 3rd character in the string is passed on 
which is the equivalent of passing a string less the first two characters 
strtoul is meant to convert string to unsigned long - library function 
*/ 
		cmd = strtoul((input+2), NULL, 10); 
/* this is meant to do error checking, but we are not sure it actually works */ 
		if(cmd == 0) {
			printf("Invalid parameter");
			return NULL;
		}
/*
We are returning a command from history, which is passed onto get_input 
and then returned again from there. 
*/
		return saved_history[(count-cmd)%20].input_line;
		}
	else {
		cmd = strtoul((input+1), NULL, 10);  
		return saved_history[cmd%20].input_line; 
	}


}

/*
this is a builtin command that just prints the user command history. Commands invoked 
from history or the invocations !## are not saved. It is not printing the commands 
in order of invocation atm i.e. cmd_no order in struct - to be fixed. 
*/  
void history(){  
	int c; 
			
	for(c = 0; c<20; c++){

		printf("%d  %s  ", saved_history[c].cmd_no, saved_history[c].input_line);
		
	}
}

char *get_input() {
	
	static int count; 
	
		
	static char input[MAXIN]; /* declared as static so it's not on the stack */
	char *cwd = getcwdir();
	bool too_much_input = true;
	int i;

	do {
		printf("[%s]%% ", cwd);
		/*Exits on ctrl+D*/
		if (fgets(input, MAXIN, stdin) == NULL) /* get user input */
			quit(); /*Exit on null pointer, given by fgets()*/
		
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
	
	/*Command history*/
	/* if (!strcspn(input, "!"));  ASK if we need to use it  - he recommends it, but we managed without it I think.. */
	
	if ('!' == input[0]) {	
		return command_history(input, count+1); 
	}

 	count++; 

	saved_history[count%20].cmd_no = count;
	strcpy(saved_history[count%20].input_line, input);
	

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
	tokens[p] = 0;
}

int internal_command(char **argv) {
	/* Internal commands */
	/* TODO: internal commands as another function */
	/* exit*/
	if (EQ(argv[0], "exit")) {
		quit();
	} else if (EQ(argv[0], "cd")) {
		cd(argv[1]);
		return 0;
	} else if (EQ(argv[0], "getpath")) {
		getpath(argv);
		return 0;
	} else if (EQ(argv[0], "setpath")) {
		setpath(argv);
		return 0;
	} else if (EQ(argv[0], "history")) {
		history(); 
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
	if (!**argv) {
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
	cd(NULL); /*Changes current working directory to HOME */
	while (1) {
		input = get_input();
		tokenise(input, argv);
		Execute(argv);
		
	}
	return 0;
}

