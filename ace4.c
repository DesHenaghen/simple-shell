/* ACE 4 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* The name of our shell! */
#define SHELLNAME "shell"
/* The number of elements in an array */
#define LEN(array) sizeof(array)/sizeof(array[0])

#define UNCHANGED 0
#define MAXIN 512 /* MAXIN is the maximum number of input characters */

#define DELIM " \n\t|><&;" /* DELIM is the string containing all delimiters for tokens */

char* get_input(char directory[]) { 
	static char input[MAXIN]; /* declared as static so it's not on the stack */
  
  	fgets(input,MAXIN,stdin); /* fgets as scanf() can't handle blank lines */
  	/* First of all check if it was a blank line, i.e. just a '\n' input...*/
  	while('\n' == input[0]) {    /*check if input is valid - i.e. not blank*/
		printf("%s >", directory); 	/*prompt user for other input*/
		fgets(input,MAXIN,stdin); 	/* get user input */ 
  	} /* TO ASK - why does fgets not return a char* in this case? */
 
  	/* If we get to this point it there has to be input so just return it. */ 
  	return(input);
}

void tokenise(char *line, char **tokens) {
	int p;
	char* token;	

	p = 0;
	token = strtok(line, DELIM); /* initial strtok call */
	while (token) { /* While there's still more... */
		tokens[p++] = token;
		token = strtok(NULL, DELIM); /* ...grab the next token */
	}
}

char* getcwdir(){
	long size;
	char *buf;
	char *ptr;

	size = pathconf(".", _PC_PATH_MAX);

	if ((buf = (char *)malloc((size_t)size)) != NULL)
	    ptr = getcwd(buf, (size_t)size);

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
	/* Let's make sure there's actually something in the array! */
	if (LEN(argv) == 0) {
		fprintf(stderr,"%s: error: no arguments given to Execute().\n",SHELLNAME);
		return(1);
	}exit


	/* The name of the command we want to run */
	char* filename = argv[0];
	
	/* Ensure our file name is properly null-terminated. */
	/* This error checking seems to be doing more harm than good. */	
	/*if (filename[LEN(filename)-1] != '\0') {
		fprintf(stderr,"%s: error: filename is not null terminated.\n",SHELLNAME);
		return(1);
	}*/
	
	/* Built-in commands */
	/* exit*/
	if(!strcmp(filename,"exit")) {
		exit(0);
	}
	
	/* TODO: The rest of the built in commands. */
	/* TODO: Search for command in path. */

	/* We couldn't find it. */
	printf("%s: %s: command not found.\n",SHELLNAME,filename);
	return(1);
}

int main() {
	chdir(getenv("HOME")); /*Changes current working directory to HOME */
	char *directory;
	char *input;
	char *argc[256];
	while (1) {
		directory = getcwdir(); /*gets current working directory*/
		input = get_input(directory);
		tokenise(input, argc);
		Execute(argc);
	}
	return 0;
}
