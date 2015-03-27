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
6. Exit on `exit` command
7. Exit on <CTRL>-d.
8. no segfaults on exit

Stage 2
-------

### Aim

Execute external commands

### Tests

1. Stage 1 still works!
2. All programs in the PATH are run as expected with various arguments.
	* Try using `ls` with different switches and varying number of files.
3. The shell shouldn't go tits up when the user enters an invalid program.

Stage 3
-------

### Aim

Set the current directory to HOME, and get and set the PATH.

### Tests

1. Stages 1 and 2 still work!
2. `cd` without arguments changes current working directory (cwd) to HOME
3. `cd` to anywhere else
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
1. The directory is changed (use `pwd`)
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
1. Type `history` and check that the last 20 commands entered are accurate
2. *Everything* should be stored in history, including garbage
3. The shell should recognise history invocations, i.e. `!<no>` or `!-<no>`, 
4. It should *not* store history invocations to history
5. The shell should execute the program indexed by the history invocation
6. Shell behaves correctly for incorrect invoations (e.g. out of range ones)

Stage 6
-------

### Aim

Persistent history

### Tests

#### Save History

0. Previous stages still work
1. .hist_list created in user's home directory
2. .hist_list contains the expected contents

#### Load History

3. history is loaded when shell is started (use history command)
4. Doesn't crash if file can't be found or file can't be opened
5. Doesn't crash is file is corrupt i.e. doesn't contain the expected contents

Stage 7
-------

### Aim

Setting up aliases, removing aliases, invoking aliases, and printing all
aliases.

### Tests

0. Previous stages still work,
1. Accepts at least 10 aliases,
2. Commands can be unaliased
3. Print 'no such alias' when user attempts to unalias non-existent alias
4. Aliases can be invoked i.e. they are replaced with the expected command
5. If no alias is found the original command line is parsed
6. `alias` command prints aliases. If there are no aliases, as message is
printed to tell the user this
7. An aliased command works with and without parameters and for both internal
and external commands
8. Garbage can be aliased. If the user aliases garbage, the shell will still
try to execute the garbage
9. Print aliases after removal check for gaps in the data structure.
10. "Correctly" handle the case when the maximum number of aliases is reached.
11. Shell doesn't go tits up when incorrect parameters are provided.

Stage 8
-------

**Branch before starting this stage:**

		git checkout -b enhanced_aliases

There is, allegedly, a high risk that this will break the previous stages.

### Aim

Enhanced aliases

### Tests

0. Previous stages still work
1. Chains of aliases without cycles and without parameters work
2. Chains of aliases without cycles but *with* parameters work
3. Chains that mix aliases and history invocation without cycles and without
parameters work
4. Chains that mix aliases and history invocation without cycles but *with*
parameters work
5. Cycles are prevented in steps 1 - 4.
