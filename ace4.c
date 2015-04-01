/* ACE 4 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

/* The name of our shell! */
#define SHELLNAME "HermitTheKermit2k6"
/* The number of elements in an array */
#define LEN(array) sizeof(array)/sizeof(array[0])
/* Are two strings equal? */
#define EQ(str1,str2) (!strcmp(str1,str2))
#define UNCHANGED 0
#define MAXIN 512 /* MAXIN is the maximum number of input characters */
#define DELIM " \n\t|><&;" /* DELIM is the string containing all delimiters for tokens */
#define SZ_ARGV 50 /* Size of the argv array */
#define HISTFILE ".hist_list"
#define MAXALIAS 10

/*Keeps count of the command number being executed*/ 
static int count;

/*A structure for storing the command number and string*/
typedef struct {
  int cmd_no;
  char input_line[MAXIN];
} history_t;

typedef struct {
    char *name;
    char *command[MAXIN];
    int num_tokens;
} alias_t;

/* An array for storing history*/
static history_t saved_history [20];
static alias_t alias[MAXALIAS];
// static int aliasCount;

const char *pathValue;

/* 	function to protect against circular aliased commands,
	calling with NULL is for the name of the most recently
	invoked history entry ("!-1" for example) or alias name,
	the history is reset by calling this function with no
	arguments, a return of 1 means it has found a previous
	call to that invokation and will be circular.
*/
int invoke_hist(char* command) {
	static char* invoked[50];
	static int num_invoked;
	int i;
	if (command == NULL) {
		for (i=0; i<num_invoked; i++)
			free(invoked[i]);
		num_invoked = 0;
		return 0;
	}

	for (i=0; i<num_invoked; i++) {
		if (strcmp(command, invoked[i])==0)
			return 1;
	}
	
	invoked[num_invoked] = malloc(sizeof(char*));
	strcpy(invoked[num_invoked++], command);
	return 0;
}

void save_history() { 
  	FILE *out;
	int i; 
	chdir(getenv("HOME"));

  if ((out = fopen(HISTFILE, "w")) == NULL) {
    perror(HISTFILE);
    return;
  }

  for (i = 1; i <= LEN(saved_history); i++) { 

		if(saved_history[i%20].cmd_no == 0)
			break;
	
		fprintf(out,"%d\n", saved_history[i%20].cmd_no);
		fprintf(out,"%s\n", saved_history[i%20].input_line);			
  }
	

  fclose(out); 
}

void load_history(){

	int i;
	int maxCount=0;
	char input[MAXIN];

	FILE* in = fopen(HISTFILE, "rw");

	if (in == NULL) {
  		printf( "Can't open input file in.list!\n");
  		return;
	}	
	while (!feof(in)) {		
		fgets(input, MAXIN, in);
		strtok(input, "\n");
		i = atoi(input);
		maxCount = (maxCount> i)? maxCount : i;
		saved_history[i%20].cmd_no = i;
		fgets(saved_history[i%20].input_line, MAXIN, in );
		strtok(saved_history[i%20].input_line, "\n");
				
	}
	count = maxCount; // TO CHANGE - needs to find the max number of instruction
	/* TODO:             ^ */

	fclose(in);
}

void quit() {
	setenv("PATH", pathValue, 1);
	printf("%s\n",getenv("PATH"));
	save_history();
	exit(0);
}

void cd(char **argv) {
	if(argv[2] == NULL){
	  	if (!argv[1]){
	    		chdir(getenv("HOME"));
		}else{
			if(chdir(argv[1]))
				perror(argv[1]);
		}
	}else{
		printf("Too many parameters. Usage: cd [directory]\n");
	}

  
}

/* Return the PATH environment variable */
void getpath(char **argv) {
	if(argv[1] == NULL){
		printf("%s\n", getenv("PATH"));
	}else{
		printf("Expects no parameters. Usage: getpath\n");
	}
}

void setpath(char **argv) {
	if(argv[2] == NULL) {
		if (argv[1] != NULL) {
			setenv("PATH", argv[1], 1);
		} else {
			printf("Setpath takes 1 parameter. Usage: setpath path\n");
		}
	}else{
		printf("Too many parameters. Usage: setpath path\n");
	}
}

/*Checks if the input is an alias or not - returns index of the alias if we find one, and -1 if not*/

int check_alias(char *name) {    
    int i;

    for(i = 0; i < MAXALIAS; i++) { /* Check all aliases */
		if(NULL == alias[i].name) { /* If it's null return a negative because there's no aliases left */
			return -1;
		}
		else if (EQ(name, alias[i].name)) { /* If we find an existing alias */
			return i; /* Return it's index */
		}	
    }
	return -1;
}

/*Checks for duplicates in the alias array and also for empty position in array - returns index of duplicate old aliases are overwritten with the newer*/

int check_alias_position(char* name)
{
	int i;
	int j;

	for(i = 0; i < MAXALIAS; i++)
	{
		if(NULL == alias[i].name)
		{
			return i;
		}
		else if (EQ(name, alias[i].name))
		{
			for(j = 0; j < SZ_ARGV; j++)
			{
				alias[i].command[j] = NULL;
			}
			return i;
		}
	}
	
	return -1;	
}



/*Prints list of current aliases*/

void printalias() {
	int i;
	int p;
	int arrayEmpty;
	
	arrayEmpty = 1;

	for (i=0; i<MAXALIAS; i++) {
		if (alias[i].name == NULL) {
			continue;
		} 
		else {
			arrayEmpty = 0;

			printf("Alias[%d]: %s: ", i, alias[i].name);
		
			for (p=0; p<MAXIN; p++) {
				if (alias[i].command[p] == NULL) {
						break;	
				}
				
				printf("%s ", alias[i].command[p]);
			}
			printf("\n");
		}	
	}

	if(arrayEmpty) {
		printf("There are no aliases to print\n");
	}
}

/*Removes an alias*/

void unalias(char **argv) {
	int i;
	int j;

	if (!argv[1]) {
		printf("Usage: unalias [name]\n");
		return;
	}

	for(i = 0; i < MAXALIAS; i++)
	{
		if(NULL == alias[i].name)
		{
			continue;
		} else if(EQ(argv[1], alias[i].name))
		{
			alias[i].name = NULL;
			free(alias[i].name);
			for(j = 0; j < SZ_ARGV; j++)
			{
				alias[i].command[j] = NULL;
				free(alias[i].command[j]);
			}
			printf("Alias removed\n");
			return;
		} 
	}

	printf("No aliases found of that name.\n");
}

/*Adds an alias by tokenising the command line input*/

void add_alias(char **argv) {
	int i, j;
	int position;

	if (!argv[1]) {
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

/*
This instruction is called when the first character of the input is a '!'
meaning a command is invoked from history
*/

char *command_history(char *input, int count) {
	int cmd;
	printf("Input is: %s\n",input);
/* '-' means counting backwards from the last commands entered */ 
	if ('-' == input[1]) { 
		cmd = strtoul((input+2), NULL, 10);
		cmd = count - cmd;
		if(cmd <= 0){
			printf("History item does not exist\n");
			return NULL;
		}
	}else {
		cmd = strtoul((input+1), NULL, 10); 
		if(cmd == 0){
			printf("History item does not exist\n");
			return NULL;
		}
	}

	if (cmd < 0 || cmd > count || cmd < count - 19){
			printf("History item does not exist\n");
			return NULL;
	}
	
	printf("%s", saved_history[cmd%20].input_line);
	return saved_history[cmd%20].input_line;
}

/*
this is a builtin command that just prints the user command history. Commands invoked 
from history or the invocations !## are not saved. It is not printing the commands 
in order of invocation atm i.e. cmd_no order in struct - to be fixed. 
*/  
void history(char **argv){  
	int c = 1; 
	if(argv[1] == NULL){
		if(count > 20)
			c = count + 1;

		/* history is saved in array starting at 1 */
		for( ; c<(count+21); c++){

			if(saved_history[c%20].cmd_no == 0)
				break;

			printf(" %d  %s\n", saved_history[c%20].cmd_no, saved_history[c%20].input_line);
		}
	}else{
		printf("History takes no parameters. Usage: history\n");
	}
}

char* get_input() {
	static char input[MAXIN]; /* declared as static so it's not on the stack */

	char *cwd = get_current_dir_name();

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

	free(cwd);

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
	
	if (!strcspn(input, "!")) {	
		return command_history(input, count); //count +1
	}

	count++;
	/*after the count++, the history starts being saved at index 1 */
	saved_history[count%20].cmd_no = count;
	input[strcspn(input, "\n")] = 0;
	strcpy(saved_history[count%20].input_line, input);
	 

	/* If we get to this point it there has to be input so just return it. */
	return (input);
}


void tokenise(char *line, char **tokens) {
	int p;
	char *token;

	/* Clear pre-existing tokens */
	for (p = 0; p < SZ_ARGV; p++)
		tokens[p] = NULL;

	p = 0;
	token = strtok(line, DELIM); /* initial strtok call */
	/* While there are more tokens and our array isn't full */
	while (token && (p < SZ_ARGV - 1)) {
		tokens[p++] = token;
		token = strtok(NULL, DELIM); /* ...grab the next token */
	}
	tokens[p] = 0;
}

/* FIXME: Have to declare Execute() before here. */
/* TODO: Make it better so we don't have to. :-P */
void Execute();

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
		tokenise(command_history(argv[0], count-1), argv);
		Execute(argv);
    } else {
	/* Return negative number if command not found */
   		return -1;
    }

    return 0;
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
	}
}

int dealias(char **argv) {
	int alias_i;

	alias_i = check_alias(argv[0]); /* Will return positive index if there is an alias in command*/

	if(alias_i >= 0) /*If the command line input is an alias*/
	{
		int i = 0, num_alias_tokens = alias[alias_i].num_tokens,
		    num_argv_tokens = 0;

		for (i = 0; i < SZ_ARGV && argv[i]; i++) {
			num_argv_tokens++;
		}

		for (i = num_argv_tokens - 1; i > 0; i--) {
			argv[i + num_alias_tokens - 1] = argv[i];
		}

		for (i = 0; i < num_alias_tokens; i++) {
			argv[i] = alias[alias_i].command[i];
		}
		return 1;
	}

	return 0;
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
	printf("hi\n");	
    if (dealias(argv)==1) {
		if (invoke_hist(argv[0])==1) {
			printf("Aborting execution to avoid circular alias/history calls\n");
			return;
		}
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
	chdir(getenv("HOME")); /* Changes current working directory to HOME */
	load_history();

	while (1) {
		invoke_hist(NULL); /* initial call to invoke_hist for a new iteration */
		if ((input = get_input())) {
			tokenise(input, argv);
			Execute(argv);
		}
	}

	return 0;
}
