/* ACE 4 */

#include <stdio.h>
#include <stdlib.h>

/* Execute looks for the command specified by filename.
 * The order it looks for the command is:
 * 	1. Among built-in commands,
 * 	2. In the PATH.
 *
 * Arguments are passed in the same way that arguments
 * are passed to C's execv function (from unistd.h), around
 * which this function is a wrapper.
 *
 * Returns 0 if successful, otherwise returns 1. */
int Execute(const char *filename, char *const argv[]) {
/* TODO: Search for built-in command. */
/* TODO: Search for command in path. */
	return 0;
}

int main() {

	return 0;
}
