/* ACE 4 */

#include <stdio.h>
#include <stdlib.h>

/* The name of our shell! */
#define SHELLNAME "shell"
/* The number of elements in an array */
#define LEN(array) sizeof(array)/sizeof(array[0])

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

	return 0;
}
