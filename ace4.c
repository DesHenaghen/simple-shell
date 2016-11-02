/*
 * Synopsis: ace4v8
 * 
 * Invoking: To invoke the programme the user must simply run the file in
 * command prompt in a Linux operating environment. This shell will run 
 * inside another or can replace an operating system shell.  
 * The program will exit if the user types 'exit' or 'ctrl+D'.  
 * 
 *  
 * Purpose: Both internal commands and system calls work. Internal commands 
 * include: exit, cd, getpath, setpath, history, alias, unalias. 
 * Command history is persistent and aliases can be set and removed, as 
 * per specification. All 8 stages complete. 
 *
 * File:  ace4v8.c
 * 
 * Authors:
 *	Irina Camilleri
 * 	Christopher Bean
 * 	Derek Clark
 * 	Desmond Henaghen
 * 	Kieran McDermott
 *
 * Group:
 *      Thu 9-11
 *
 *
 * Version: See VERSION below
 *
 *
 **************************************************************************/

#define VERSION "ace4v8  Last Update 02/04/2015 \n"
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

/* The number of elements in an array */
#define LEN(array) sizeof(array)/sizeof(array[0])
/* Are two strings equal? */
#define EQ(str1,str2) (!strcmp(str1,str2))
#define UNCHANGED 0
/* MAXIN is the maximum number of input characters */
#define MAXIN 512
/* DELIM is the string containing all delimiters for tokens */
#define DELIM " \n\t|><&;"
/* Size of the argv array */
#define SZ_ARGV 50
/* A file that is not visible in the directory, but is used to make 
history persistent */
#define HISTFILE ".hist_list"
/* The maximum number stored as per specification */
#define MAXALIAS 10

/*Keeps count of the command number being executed*/
static int count;

/*Structure for storing history - i.e. the command number and string 
and one for aliases */
typedef struct {
	int cmd_no;
	char input_line[MAXIN];
} history_t;

typedef struct {
	char *name;
	char *command[MAXIN];
	int num_tokens;
} alias_t;

/* Array for storing history & aliases*/
static history_t saved_history[20];
static alias_t alias[MAXALIAS];

const char *pathValue;

/* Synopsis:
 * int invoke_hist(char* command)
 *
 * Description:
 * Function to protect against circular aliased commands, calling
 * with NULL is for the name of the most recently invoked history
 * entry ("!-1" for example) or alias name, the history is reset by
 * calling this function with no arguments, a return of 1 means it
 * has found a previous call to that invokation and will be circular.
 */


int invoke_hist(char* command) {
	static char* invoked[50];
	static int num_invoked;
	int i;
	/* if the command supplied is NULL then reset everything */
	if (command == NULL) {
		for (i = 0; i < num_invoked; i++)
			free(invoked[i]);
		num_invoked = 0;
		return 0;
	}
	/* Otherwise search the array of previous commands */
	for (i = 0; i < num_invoked; i++) {
		/* If we find that this command was called before then... */
		if (strcmp(command, invoked[i]) == 0)
			/* return 1 to indicate circular dependency */
			return 1;
	}
	/* If we reach here then it's fine to execute this command... */
	/* ...but add it to the array of previous commands */
	invoked[num_invoked] = malloc(sizeof(char*));
	strcpy(invoked[num_invoked++], command);
	return 0;
}

/* Synopsis:
 * 
 * In order to make history persistent, it is saved to a file 
 * via this function before exiting.  
 */

void save_history() {
	FILE *out;
	int i;
	chdir(getenv("HOME"));

	if ((out = fopen(HISTFILE, "w")) == NULL) {
		perror(HISTFILE);

		if (out == NULL)
			return;
	/* if failed to open file to save, don't continue */
	}

	for (i = 1; i <= LEN(saved_history); i++) {

		if (saved_history[i % 20].cmd_no == 0)
			break;

		fprintf(out, "%d\n", saved_history[i % 20].cmd_no);
		fprintf(out, "%s\n", saved_history[i % 20].input_line);
	}

	fclose(out);
}

/* Synopsis:
 * 
 * To get the previous commands entered, history is loaded from the file. 
 */

void load_history() {

	int i;
	int maxCount = 0;
	char input[MAXIN];

	FILE* in = fopen(HISTFILE, "rw");

	if (in == NULL) {
		printf("Can't open input file in.list!\n");
		return;
	/*If failed to locate file, don't go past this point*/
	}
	while (!feof(in)) { /*load all file contents into history array */
		fgets(input, MAXIN, in);
		strtok(input, "\n");
		i = atoi(input);
		maxCount = (maxCount > i) ? maxCount : i;
		saved_history[i % 20].cmd_no = i;
		fgets(saved_history[i % 20].input_line, MAXIN, in);
		strtok(saved_history[i % 20].input_line, "\n");

	}
	count = maxCount;

	fclose(in);
}


/* Synopsis:
 * 
 * Internal command - quit, which accounts for restoring path and 
 * saving recent history, before exiting.  
 */

void quit() {
	setenv("PATH", pathValue, 1);
	printf("%s\n", getenv("PATH"));
	save_history();
	exit(0);
}

/* Synopsis:
 * 
 * Internal command - change directory.   
 */

void cd(char **argv) {
	if (argv[2] == NULL) {
		if (!argv[1]) {
		/*if only 'cd' was entered - no other parameters*/
			chdir(getenv("HOME"));
		} else {
			if (chdir(argv[1]))
				perror(argv[1]);
		}
	} else {
		printf("Too many parameters. Usage: cd [directory]\n");
	}

}

/* Synopsis:
 * 
 * Internal command - Return the PATH environment variable 
 */
void getpath(char **argv) {
	if (argv[1] == NULL) {
		printf("%s\n", getenv("PATH"));
	} else {
		printf("Expects no parameters. Usage: getpath\n");
	}
}

/* Synopsis:
 * 
 * Internal command - Set the PATH environment variable 
 */

void setpath(char **argv) {
	if (argv[2] == NULL) {
		if (argv[1] != NULL) {
			setenv("PATH", argv[1], 1);
		} else {
			printf("Setpath takes 1 parameter. Usage: setpath path\n");
		}
	} else {
		printf("Too many parameters. Usage: setpath path\n");
	}
}

/* Checks if the input is an alias or not - returns index of the
 * alias if we find one, and -1 if not */

int check_alias(char *name) {
	int i;

	/* Check all aliases */
	for (i = 0; i < MAXALIAS; i++) {
		if (NULL == alias[i].name) {
			continue;
		} else if (EQ(name, alias[i].name)) {
		/* If name matches an alias name then return the index */
			return i;
		}
	}
	return -1;
}

/* Checks for duplicates in the alias array and also for empty
 * position in array - returns index of duplicate old aliases
 * to be overwritten with the newer entry
 */
int check_alias_position(char* name) {
	int i;
	int j;

	for (i = 0; i < MAXALIAS; i++) {
		if (NULL == alias[i].name) {
			return i;
		} else if (EQ(name, alias[i].name)) {
			for (j = 0; j < SZ_ARGV; j++) {
				alias[i].command[j] = NULL;
			}
			return i;
		}
	}

	return -1;
}


/* Prints list of current aliases. Invoked when 
 * the user enters 'alias' with no parameters following. 
 * Check for this is in add_alias function and that is where it is 
 * invoked.  
 */

void printalias() {
	int i;
	int p;
	int arrayEmpty;

	arrayEmpty = 1;
	/* For every non-NULL alias name... */
	for (i = 0; i < MAXALIAS; i++) {
		if (alias[i].name == NULL) {
			continue;
		} else {
			arrayEmpty = 0;

			/* ...print the name... */
			printf("Alias[%d]: %s: ", i, alias[i].name);

			/* ...and all the command tokens */
			for (p = 0; p < MAXIN; p++) {
				if (alias[i].command[p] == NULL) {
					break;
				}
				printf("%s ", alias[i].command[p]);
			}
			printf("\n");
		}
	}

	if (arrayEmpty) {
		printf("There are no aliases to print\n");
	}
}

/* Synopsis:
 * 
 * Internal command - Removes an alias 
 */
void unalias(char** argv) {
	int i;
	int j;

	if (!argv[1]) {
		printf("Usage: unalias [name]\n");
		return;
	}	

	/* For every non-NULL alias... */
	for (i = 0; i < MAXALIAS; i++) {
		if (NULL == alias[i].name) {
			continue;
		
		} else if (EQ(argv[1], alias[i].name)) {
			alias[i].name = NULL;

			free(alias[i].name);

			for (j = 0; j < SZ_ARGV; j++) {
				alias[i].command[j] = NULL;
				free(alias[i].command[j]);
			}
			printf("Alias removed\n");
			return;
		}
	}

	printf("No aliases found of that name.\n");
}

/* Synopsis:
 * 
 * Internal command - adding an alias to a max list of 10.  
 */

void add_alias(char **argv) {
	int i, j;
	int position;

	if (!argv[1]) {
	/*if command entered 'alias' was not followed by parameters, 
	print the list of 'alias' instead - as per specification*/ 
		printalias();
		return;
	} else if (!argv[2]) {
		printf("Usage: alias [name] [command]\n");
		return;
	}

	if ((position = check_alias_position(argv[1])) < 0) {
		printf("You're storing too many aliases. Remove some to add more!\n");
		return;
	}

	alias[position].name = (char *) malloc(sizeof(char*));
	strcpy(alias[position].name, argv[1]);

	alias[position].num_tokens = 0;

	for (i = 2, j = 0; i < SZ_ARGV && argv[i]; i++, j++) {
		alias[position].command[j] = (char *) malloc(sizeof(char*));
		strcpy(alias[position].command[j], argv[i]);
		alias[position].num_tokens++;
	}

}

/* This instruction is called when the first character of the input
 * is a '!' meaning a command is invoked from history. */

char *command_history(char *input, int count) {
	int cmd;

	/* '-' means counting backwards from the last commands entered */
	if ('-' == input[1]) {
		cmd = strtoul((input + 2), NULL, 10);
		cmd = count - cmd;
		if (cmd <= 0) {
			printf("History item does not exist\n");
			return NULL;
		}
	} else {
		cmd = strtoul((input + 1), NULL, 10);
		if (cmd == 0) {
			printf("History item does not exist\n");
			return NULL;
		}
	}
 /* If history number is less than zero or greater than the current */
 /* instruction number or more than 20 instructions ago then history item */
 /* doesn't exist */
	if (cmd < 0 || cmd > count || cmd < count - 19) {
		printf("History item does not exist\n");
		return NULL;
	}

	strcpy(input, saved_history[cmd % 20].input_line);
	return input;
}

/* Synopsis:
 * 
 * Internal command - just prints the user command history. 
 * Commands invoked from history or the invocations !## are
 * not saved.
 */
void history(char **argv) {
	int c = 1;
	if (argv[1] == NULL) {
		if (count > 20)
			c = count + 1;

		/* history is saved in array starting at 1 */
		for (; c < (count + 21); c++) {

			if (saved_history[c % 20].cmd_no == 0)
				break;

			printf(" %d  %s\n", saved_history[c % 20].cmd_no,
					saved_history[c % 20].input_line);
		}
	} else {
		printf("History takes no parameters. Usage: history\n");
	}
}

/* Get input from the terminal */

char* get_input() {
	static char input[MAXIN];
	char *cwd = get_current_dir_name();
	bool too_much_input = true;
	int i;

	/* Using fget to get the input line and performing
	input validation and sanity checks */

	do {
		printf("[%s]%% ", cwd);
		if (fgets(input, MAXIN, stdin) == NULL)
			/* Exit on null pointer returned from fgets() */
			quit(); 
	} while ('\n' == input[0]);

	free(cwd);
	
	/* Ensure the input is not greater than 'MAXIN' */
	for (i = 0; i < MAXIN && input[i] != '\0'; i++) {
		if (input[i] == '\n')
			too_much_input = false;
	}
	if (too_much_input) {
		/* Flush the excess input from stdin */
		while (getchar() != '\n')
			;
	}

	/* Save the valid input to history */
	count++;
	saved_history[count % 20].cmd_no = count;
	input[strcspn(input, "\n")] = 0;
	strcpy(saved_history[count % 20].input_line, input);

	/* Finally return the valid input */
	return (input);
}


/* Converts the string 'line' into an array of tokens, 'tokens'. 
 * 
 * NOTE: This destroys the string 'line'. 
 */

void tokenise(char *line, char **tokens) {
	int p;
	char *token;

	/* Clear pre-existing tokens */
	for (p = 0; p < SZ_ARGV; p++)
		tokens[p] = NULL;

	p = 0;
	token = strtok(line, DELIM); /* initial strtok call */

	/* While there are more tokens and our array isn't full, add the
	 * token to tokens and get the next token. */
	while (token && (p < SZ_ARGV - 1)) {
		tokens[p++] = token;
		token = strtok(NULL, DELIM);
	}
	
	/* Null-terminate tokens. */
	tokens[p] = 0;
}


/* Stub to suppress warnings of the execute command 
 * being called before it is declared. 
 * Internal_commands reference execute and execute 
 * references internal command. 
 */
void Execute();


/* All internal commands except from history are handled here */
int internal_command(char **argv) {

	if (EQ(argv[0], "exit")) {
		quit();
	} else if (EQ(argv[0], "cd")) {
		cd(argv);
	} else if (EQ(argv[0], "getpath")) {
		getpath(argv);
	} else if (EQ(argv[0], "setpath")) {
		setpath(argv);
	} else if (EQ(argv[0], "history")) {
		history(argv);
	} else if (EQ(argv[0], "alias")) {
		add_alias(argv);
	} else if (EQ(argv[0], "unalias")) {
		unalias(argv);
	} else if (!strcspn(argv[0], "!")) {
		tokenise(command_history(argv[0], count - 1), argv);
		Execute(argv);
	} else {
		/* Return negative number if command not found */
		return -1;
	}

	return 0;
}

/* If the user command entered was not internal, 
 * make a system call 
 */ 
void external_command(char **argv) {
	pid_t pid;

	/* fork a child process */
	pid = fork();
	if (pid < 0) {
		/* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return;
	} else if (pid == 0) {
		/* child process */
		execvp(argv[0], argv);
		perror(argv[0]);
		exit(0);
	} else {
		/* parent process */
		/* parent will wait for the child to complete */
		wait (NULL);
	}
}

/* Dealiases the first element in argv. If the alias dealiases to a
 * command with more than one token then it moves everything in argv
 * to a higher index in order to accommodate the new tokens. Then
 * copies the new tokens into the first indexes of argv. */



int dealias(char **argv) {
	int alias_i;
	/* Will return positive index if there is an alias in command*/
	alias_i = check_alias(argv[0]);

	/*If the command line input is an alias*/
	if (alias_i >= 0) {
		int i = 0, num_alias_tokens = alias[alias_i].num_tokens,
				num_argv_tokens = 0;

		/* Get the number of tokens in argv */
		for (i = 0; i < SZ_ARGV && argv[i]; i++)
			num_argv_tokens++;

		/* Move everything in argv up the array to 
		 * accommodate the tokens in alias[alias_i] */
		for (i = num_argv_tokens - 1; i > 0; i--)
			argv[i + num_alias_tokens - 1] = argv[i];

		/* put the tokens in alias into the start argv */
		for (i = 0; i < num_alias_tokens; i++)
			strcpy(argv[i], alias[alias_i].command[i]);

		return 1;
	}

	return 0;
}



/* Execute looks for the command specified by filename.
 * The order it looks for the command is:
 * 	1. Among built-in commands,
 * 	2. In the PATH.
 *
 * If the command entered is an alias then it gets recursively
 * stripped down until it can execute succesfully.
 * Arguments are passed as an array where the first element
 * is the name of the command we want to run and the following
 * elements are arguments to that command.
 */

void Execute(char *argv[]) {
	char *history;
	/* First check if the command entered is stored as an alias */
	if (dealias(argv) == 1) {
		/* Making sure we're not executing the same thing again */
		if (invoke_hist(argv[0]) == 1) {
			printf("Aborting execution to avoid circular alias/history calls\n");
			return;
		}
		/* Recurse with the aliased command */
		Execute(argv);
		return;
	}

	if (!strcspn(argv[0], "!")) {
		history = command_history(argv[0], count);
		tokenise(history, argv);
		Execute(argv);
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
	chdir(getenv("HOME"));
	load_history();
	/*continuous loop, which is exited only via the 
	inbuilt 'exit' function */
	while (1) {
		/* Initial call to invoke_hist() for a new iteration */
		invoke_hist (NULL);
		if ((input = get_input())) {
			tokenise(input, argv);
			if (argv[0])
				Execute(argv);
		}
	}

	return 0;
}

    
