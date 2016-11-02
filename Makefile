CFLAGS = -pedantic -Wall -std=c99

all: shell

shell: shell.c
	$(CC) $(CFLAGS) -o shell shell.c

clean:
	$(RM) shell
