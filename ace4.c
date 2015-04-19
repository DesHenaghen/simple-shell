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
 *      Irina Camilleri, Reg no: 201330546
 * 	Christopher Bean, Reg no: 201328557 
 * 	Derek Clark, Reg no: 201343086
 * 	Desmond Henaghen, Reg no: 201320274
 * 	Kieran McDermott, Reg no: 201305763
 *
 * Group:
 *      Thu 9-11
 *
 *
 * Version: See VERSION below
 *
 *
 **************************************************************************/

/********************* R E V I S I O N   H I S T O R Y ********************

commit 2226ad7d8dc45544a89b8e598e2fe46b6ca56c1f
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Apr 2 11:05:49 2015 +0100

    Ironing. I changed history_line_t to history_t to match alias_t. 
Please don't assassinate me :-3 Fix unalias

commit bac4983433561e97adc77678b72f9c4409a71b54
Author: derek <derekclark1995@yahoo.co.uk>
Date:   Wed Apr 1 23:49:15 2015 +0100

    Removed debug print statements and improved comments in some areas. 
Circular dependency will no longer result in a segmentation fault

commit 40ce9e04dda069d1a833299580b0c619eda0cdea
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Wed Apr 1 19:52:57 2015 +0100

    Added fix for history invocations longer than one word.
    

commit a65f90f57c3b62fa261b0be012603243a1e32102
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Wed Apr 1 13:09:47 2015 +0100

    Add alias and execute alias with parameters

commit 50e9a5b9ab05ebda24a542e4355ecdf5dc9a926a
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Wed Apr 1 12:11:28 2015 +0100

    I made alias now accept history invocations. 
Some slight warnings pop up though

commit 9a6e15c582e1bc0ef58efe891f7dd04d1c851da7
Author: Kieran <kieryweery@gmail.com>
Date:   Mon Mar 30 20:55:51 2015 +0100

    Changed the printing of aliases to give appropriate message when there 
were no aliases to print, added a message to tell the user there are too many 
aliases, now only allocate memory when there is space in the array

commit 4ee52b49bb9d9b7f714d4f65ba2441ff67b220c1
Author: Kieran <kieryweery@gmail.com>
Date:   Sun Mar 29 23:32:05 2015 +0100

    Refactored the tokenise function as it was overcrowded, moved the adding 
of aliases out into a separate function, changed the way we add an alias - 
now an alias is added at the first null position in the array. Also added 
the checking of duplicates so that newer aliases overwrite older aliases 
with the same name. Altered the unalias function due to the other changes 
I have made - still not fully working but it's getting close

commit 172a4ae0b6563191d3f1acee030bb6f9c7d54d3a
Author: derek <derekclark1995@yahoo.co.uk>
Date:   Sun Mar 29 17:50:17 2015 +0100

    printalias and unalias implemented, unalias not currently working correctly

commit ca04d058905d9cd748ae7dbdab065b7788769b9e
Author: Kieran <kieryweery@gmail.com>
Date:   Sun Mar 29 14:57:19 2015 +0100

    alias code completely changed and now stores a full command. 
Hopefully works with any command, some code was altered so this part may need 
to be thoroughly tested to check I haven't destroyed anyone elses code. 
Should be fine though..

commit a0e65b8a3aaba74ff24e85022735c036b38c3a62
Author: Kieran <kieryweery@gmail.com>
Date:   Fri Mar 27 10:09:36 2015 +0000

    Changed function names to conform to naming convention

commit 859a4b06d83b3b1e47171f7bd8caa0dcfa42ec3e
Author: Kieran <kieryweery@gmail.com>
Date:   Fri Mar 27 01:52:46 2015 +0000

    Added error checking on alias command and an appropriate error message

commit 6307c121d15ab2c7e55ce095edfeb0d40a009d72
Author: Kieran <kieryweery@gmail.com>
Date:   Thu Mar 26 21:56:06 2015 +0000

    Aliases now fully working for a single command, also now accept paramaters. 
Storing an alias with more than one paramater still a problem so I have removed
it for now

commit d84312073bb878c7b10544e5189cc9f08ba0259e
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Thu Mar 26 12:28:53 2015 +0000

    Adding validation to history so it doesn't accept parameters

commit a6b589ed342e1f7b34b72e65f38adb90c4416765
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Thu Mar 26 12:16:45 2015 +0000

    I fixed the history printing so it now prints instructions in order, 
save/load history now works for .hist_list, I removed getcwdir() and replaced 
it with get_current_dir_name() as it's simpler and allocates memory for us, 
meaning an extra function isn't needed, history was also changed so that 
History is accessed by the current command_no - the number entered, and not
 the next prospective command number entered - number entered

commit fdefee72bb31e24d02f5388a9518450ad615e897
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Tue Mar 24 19:13:42 2015 +0000

    I added checks for setpath and cd to check for multiple parameters, 
and then reject them. Had a problem with cd as argv[2] was notbeing 
recognised as NULL, so had to shoehorn a fix into tokenise. 
If anyone has a more elegant idea, change away!

commit 3f0e9a02545d816355e8cfe148b44f48f3f40c68
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Tue Mar 24 19:08:51 2015 +0000

    Work in progress: added a list of final tests to be carried out before submission.

commit 19841fb06f320ba73302cea8b7e4fea5fa196748
Author: Ira <irinacamilleri@gmail.com>
Date:   Tue Mar 24 15:30:51 2015 +0000

    Stages 5 & 6 working - all bugs fixed. History is saved in the array 
starting at index 1, to match the actual history numbering. All code has 
been changed to reflect this - all loops start at 1 and %20 for length to 
get to 0 at the end. That gets rid of adding and subtracting 1s in parameters.

commit d0ad9332f34cb02eb602a3f42032b0f2d3749f18
Author: Kieran <Kieran@Kierans-MacBook-Pro.local>
Date:   Tue Mar 24 12:07:56 2015 +0000

    Alias stuff - works only with one command for now \!

commit b0d7ee3e3da01831b8b10a9d15b562e6b151ab07
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Wed Mar 18 12:12:53 2015 +0000

    Added load history. Not currently working

commit 272274971ec62f38a1486f884202d8a2cb2e47d1
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Mar 12 22:47:12 2015 +0000

    Save .hist_list in user's home directory.

commit 3afd98f858cfa8b8e3080fa537596f5b7ff2363d
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Mar 12 11:31:31 2015 +0000

    Fixed warnings. Save_history doesn't segfault (as far as it's been tested).

commit 09e6eb6e4276c456028c1101fb28b07913146ea3
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Thu Mar 12 10:46:28 2015 +0000

    Fixed validation for command history and changed the array indexing 
for the history to be count-1%20

commit f8b6738e5f329a1d6c649922990bf5dd76f44631
Author: Ira <irinacamilleri@gmail.com>
Date:   Tue Mar 10 12:17:56 2015 +0000

    Added save_history function for stage 6 - persistent history. Untested.

commit 9ffa58e64ff759b047d9786521448ee15086a0cf
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Tue Mar 10 12:13:39 2015 +0000

    Fixed history problem

commit 7f39975060443a3c62ed9ec0b697c03cc477e9e0
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Tue Mar 10 11:22:47 2015 +0000

    Properly fixed the memory leak and added a note to the offending 
function to remind future programmers to free() the pointer. 
made cd more neat. Squashed some bugs.
  

commit ccee923a618180bb2e1618838fb4b547d800a407
Author: derek <derekclark1995@yahoo.co.uk>
Date:   Thu Mar 5 22:36:10 2015 +0000

    fixed a small error

commit be970db1a07ac47bed6e029e472fd0417b6f647e
Author: Ira <irinacamilleri@gmail.com>
Date:   Thu Mar 5 20:21:23 2015 +0000

    Included code for saving all the commands in history inside the 
get_input function before they are returned. Using and array of struct 
with int and String for this. The main code of invoking from history has 
been separated into a new function - command_history. A new builtin 
history function which prints the array of saved commands added. 
It has been incorporated into the code and can be called. 
No error checking and bugs persist in this version.

commit 42c11a99357cea042b8187c904a826c0c2f603f6
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Thu Mar 5 10:36:42 2015 +0000

    Added error checking to getpath and setpath for parameters passed in

commit 76778a54ddfa9d8dd425ae0347ef79ee06a34a4a
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Feb 26 21:12:44 2015 +0000

    Little improvement to clearing lines that are too long in get_input(). 
More efficienct by doing away with the otherwise unused variable c and 
breaking on a '\n' as well as a '\0'.

commit f8b9c02d183944072399836345673514e8166f59
Author: derek <derekclark1995@yahoo.co.uk>
Date:   Thu Feb 26 20:00:34 2015 +0000

    code cleanup and -pedantic and -Wall flags now with no warnings

commit d90e36d706603cfc05c0422192b1bcfc42ec04f9
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Feb 26 16:05:10 2015 +0000

    Moved getcwdir() to before get_input() so that it can be used for 
displaying the current directory in the prompt. get_input() now uses getcwdir() 
to get the current directory so no longer takes the directory as an argument. 
Added some sanity checking to the call to setpath(), namely to make sure 
that argv has at least 2 elements. Removed fairly useless print_error() 
function that I added in the last commit.

commit ee177def756e9794f156cca438b9421d7c9e7283
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Feb 26 13:06:21 2015 +0000

    Renamed shell. Made stdin clearing slightly more efficient. 
Fixed code formatting: code is now properly aligned.

commit 6708211677466b808b7dde846fdd6cbf5c4e12cf
Author: Ira <irinacamilleri@gmail.com>
Date:   Thu Feb 26 13:00:07 2015 +0000

    Added flushing of the terminal in the get_input method, so if there 
are more that 512 characters in the terminal - i.e. the required limit, 
the rest of the stdin is flushed

commit 2780553f63124a97e7f1bdb88a41ce37268b299c
Author: Kieran <kieryweery@gmail.com>
Date:   Thu Feb 26 12:54:25 2015 +0000

    Set path method fixed. Outputs error message when null path entered. 

commit 9065118ef59dc8fd97983e397111e24a24a4141a
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Feb 26 12:24:26 2015 +0000

    Quashed errant newline. ಠ_ರೃ

commit 67b15bb6f8a38711ed73d0afc86fe22ffbaae7b5
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Feb 26 12:14:54 2015 +0000

    Ensure tokens array does not overflow.

commit 5a134f582b112a5269236994461f850b57c63eb8
Author: Ira <irinacamilleri@gmail.com>
Date:   Thu Feb 26 12:01:31 2015 +0000

    Changed the parameter names to 'argv' and shortened the array length 
to 50 as per specification

commit d79f9a0be9a4ca8f44ea03b37af9a7bc1450198c
Author: Kieran <kieryweery@gmail.com>
Date:   Thu Feb 26 12:01:04 2015 +0000

    Added set path, not giving errors yet though...

commit 44bd9bd7e0d16f66fe8400b8a7c44e60ac7082e8
Author: Ira <irinacamilleri@gmail.com>
Date:   Thu Feb 26 11:53:43 2015 +0000

    Added code to null terminate the array when it is tokenised. 
External system calls now work!

commit 17bb18d7bb7b9dd4af384658c5e642e4ab0224e9
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Feb 26 11:31:00 2015 +0000

    Fixed error in string comparison in Execute().

commit c2e50e1d6abf305d2e74494377c0738544ba2954
Author: Ira <irinacamilleri@gmail.com>
Date:   Thu Feb 26 11:19:19 2015 +0000

    Execute function now forks and includes the execpv function to 
run external commands. Work in progress.

commit 7421ad3c1bcf0bfdbc59167eed040267023551f9
Author: Kieran <kieryweery@gmail.com>
Date:   Thu Feb 26 11:16:05 2015 +0000

    Added quit function, if you need to quit YOU MUST USE THIS FUNCTION 
otherwise you will kill your pc. We added the setting of the path value 
and then in quit() the path value is restored.

commit bd7ed73809c3a00a8f0ac1a8d1128d62c382bda8
Author: Kieran <kieryweery@gmail.com>
Date:   Thu Feb 26 10:41:54 2015 +0000

    Added Control + D to exit

commit 36b90642436b8f57d327e49711df2983b7a274fe
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Thu Feb 26 10:18:56 2015 +0000

    Deleted stray exit :P

commit 95197586b3cedbfb5e6a1f34f1fe6a273cf108dd
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Thu Feb 26 10:07:13 2015 +0000

    Removed gethomedir() function and used getenv() instead. 
Added getcwdir function which implements getcwd

commit d79b514ed9a7e9cc722000d2bde4166400882fad
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Thu Feb 26 09:52:17 2015 +0000

    #defined EQ to compare 2 strings. Implemented getpath.

commit fe4239de11b752dbf619fa62fa452211e87b34c4
Author: Ira <irinacamilleri@gmail.com>
Date:   Thu Feb 26 09:13:50 2015 +0000

    Changed the body of get_input function to a 'do while' loop, 
so directory is printed before every user input

commit ee5d9e808e60bd179f60e997243f18007238acd8
Author: derek <derekclark1995@yahoo.co.uk>
Date:   Wed Feb 25 18:49:48 2015 +0000

    changed tokenise return type to void

commit 2c96ff1cc35101fc46c3a19909fa7ba665c2c731
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Wed Feb 25 18:06:33 2015 +0000

    Fixed bug in Execute where string is incorrectly identified 
as having a null terminator.

commit c5984234447cf56e2e532bdf275b489abb7a4009
Author: derek <derekclark1995@yahoo.co.uk>
Date:   Wed Feb 25 17:44:04 2015 +0000

    tokeniser complete

commit 30953b7a820a930210f4818876a5766154487d9f
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Wed Feb 25 14:28:14 2015 +0000

    Fixed a little bug in getHomeDir() and another in main. 
Added call to Execute() in main so that it tries to execute the command 
passed to it from get_input (it will be passed from the tokeniser 
once the tokeniser's operational). Implemented "exit" in Execute().

commit 15e7e37a8512b3cdc2a31b174e1f1712a240e77e
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Wed Feb 25 13:22:55 2015 +0000

    Changed the way arguments are passed to Execute(): arguments are 
passed in an array in which the first element is the name of the command 
we want to run and the following elements are the arguments that will 
be passed to that command. Implemented some basic error checking for 
the Execute() function: ensure that the array passed to it contains at 
least one element and that the name of the command we want to run is 
null-terminated. It must be null-terminated to prevent any potential 
mis-haps with strcmp. Print some error messages if anything is wrong.

commit b772521462ddce60acc370db11302da21c087a10
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Wed Feb 25 11:54:21 2015 +0000

    Edited getHomeDir() so that it now returns a char* and this has been 
implemented in main with the result of the function being saved in the 
variable dictionary

commit 5ed09b204aa79fe39530cf8687f31aee8989e65c
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Tue Feb 24 22:54:53 2015 +0000

    Removed file ending in '~' and added line to .gitignore to prevent 
it happening again.

commit ecbd0d2c0d933580f500d0d19600769ec0eaef4e
Author: Desmond Henaghen <deshenaghen@yahoo.co.uk>
Date:   Tue Feb 24 21:59:00 2015 +0000

    Added initial working director

commit 991e9d93bfe04f9af41f49d7bd3b2cedc49ca21d
Author: Ira <irinacamilleri@gmail.com>
Date:   Tue Feb 24 19:47:01 2015 +0000

    Included the get_input function for accepting user input. 
Based on 'fgets' to avoid buffer overflow errors. The function accepts 
a string which is the current directory path and includes input validation
 which reprompts for user input if it's a blank line. Directory path 
is reprinted on screen.  Basically, the function only ever returns actual 
string input for further validation- no blanks.

commit e91e5297b473d241122ac1c9761a1467e80fc5b2
Author: Chris Bean <chrisdwbean@gmail.com>
Date:   Mon Feb 23 22:10:02 2015 +0000

	Initial commit. Empty main method!    
Execute function, with much documentation and little functionality. For now.

*****************************************************************************/

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

	/* declared as static so it's not on the stack */
	static char input[MAXIN];

	char *cwd = get_current_dir_name();

	bool too_much_input = true;
	int i;

	/* fgets as scanf() can't handle blank lines */
	/* check if it was a blank line, i.e. just a '\n' input...*/
	/*check if input is valid - i.e. not blank*/

	do {
		printf("[%s]%% ", cwd);
		/*Exits on ctrl+D*/
		if (fgets(input, MAXIN, stdin) == NULL) /* get user input */
			quit(); /*Exit on null pointer, given by fgets()*/

	} while ('\n' == input[0]);

	free(cwd);

	/* If the line was longer than the input array... flush terminal */
	for (i = 0; i < MAXIN && input[i] != '\0'; i++) {
		switch (input[i]) {
		case '\n':
			too_much_input = false;
			break;
		}
	}
	/* ...clear the rest of it */
	if (too_much_input) {
		while (getchar() != '\n')
			;
	}
 	/*if it was a history invocation */

	if (!strcspn(input, "!")) {
		return command_history(input, count);
	}

	/*after the count++, the history starts being saved at index 1 */
	count++;
	/*If you get to this point, it is a valid command, so save it to 
	command history */

	saved_history[count % 20].cmd_no = count;
	input[strcspn(input, "\n")] = 0;
	strcpy(saved_history[count % 20].input_line, input);

	/* If we get to this point it there has to be input so just return it. */
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
			Execute(argv);
		}
	}

	return 0;
}

    
