/* ACE 4 */

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>

/* The name of our shell! */
#define SHELLNAME "shell"
/* The number of elements in an array */
#define LEN(array) sizeof(array)/sizeof(array[0])

#define UNCHANGED 0
#define MAXIN 512 /* MAXIN is the maximum number of input characters */


const char* get_input(char directory[]) { 
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

char* getHomeDir(){
	uid_t uid = getuid(); /*gets current users ID*/
	struct passwd *pw = getpwuid(uid); /*gets pw struct of current user*/
	
	if(pw == null) /*Basic error checking*/
		printf("ERROR\n");

	return pw->pw_dir;/*returns initial working directory*/
}

/* Execute looks for the command specified by filename.
 * The order it looks for the command is:
 * 	1. Among built-in commands,
 * 	2. In the PATH.
 *
 * Arguments are passed as an array where the first element
 * is the name of the command we want to run and the following
 * elements are arguments to that command. */
void Execute(const char *argv[]) {
	/* Let's make sure there's actually something in the array! */
	if (LEN(argv) == 0) {
		fprintf(stderr,"%s: error: no arguments given to Execute().\n",SHELLNAME);
		return;
	}

	/* The name of the command we want to run */
	const char* filename = argv[0];
	
	/* Ensure our file name is properly null-terminated. */
	if (filename[LEN(filename)] != '\0') {
		/* It isn't?! Let us know on stderr. */
		fprintf(stderr,"%s: error: filename is not null terminated.\n",SHELLNAME);
		return;
	}
	
	/* Built-in commands */
	
	/* TODO: The rest of the built in commands. */

	/* TODO: Search for command in path. */

	/* We couldn't find it. */
	printf("%s: %s: command not found.\n",SHELLNAME,filename);
	return;
}

int main() {
	char directory[] = getHomeDir(); /*gets initial working directory*/
	printf("%s", get_input(directory));
	return 0;
}
