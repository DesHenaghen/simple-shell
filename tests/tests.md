List of Tests for the Simple Shell
===================================

Author: Christopher Bean

This aims to be a comprehensive checklist for the final testing of the shell.

Stage 1 
-------

### Aim

Prompt user, read and parse user input, exit shell, and initialise the
working directory.

### Tests

1. Get input of at least 512 characters
2. Mustn't crash upon entering *more* than 512 charcters
3. Tokenise string delimited by ' ', '\t', '|', '>', '<', '&' and ';'.
4. Accepts at least 50 tokens.
5. Shell doesn't crash when user enters empty string
6. Exit on 'exit' command
7. Exit on 'C-d'.
8. no segfaults on exit

Stage 2
-------

### Aim

Execute external commands

### Tests

1. Stage 1 still works!
2. All programs in the PATH are run as expected with various arguments.
	* Try using 'ls' with different switches and varying number of files.
3. The shell shouldn't go tits up when the user enters an invalid program.

Stage 3
-------

### Aim

Set the current directory to HOME, and get and set the PATH.

### Tests

1. Stages 1 and 2 still work!
2. 'cd' without arguments changes current working directory (cwd) to HOME
3. 'cd' to anywhere else
4. PATH is restored to its original value before exiting the shell
5. setpath changes the path
6. getpath shows the current path
7. program behaves correctly when incorrect parameters are provided to getpath
   and setpath

Stage 4
-------

### Aim

Changing directory

*cd - change working directory*

### Tests

0. Previous stages still work.
1. The directory is changed (use 'pwd')
	* Actually since our shell prints the current directory in the prompt
	  this is unneccessary
2. Shell should behave correctly when incorrect parameters are given to the
   command.

Stage 5
-------

### Aim

Adding commands to history, invoking commands from history and printing the
history.

### Tests

0. Previous stages still work.
1. Type 'history' and check that the last 20 commands entered are accurate
2. *Everything* should be stored in history, including garbage
3. The shell should recognise history invocations, i.e. !<no> or !-<no>, 
4. It should *not* store history invocations to history
5. The shell should execute the program indexed by the history invocation
6. Shell behaves correctly for incorrect invoations (e.g. out of range ones)

Stage 6
-------

### Aim

Persistent history

### Tests
