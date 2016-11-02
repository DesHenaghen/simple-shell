CFLAGS = -pedantic -Wall -std=c99

all: shell

shell: ace4.c
	$(CC) $(CFLAGS) -o shell ace4.c

clean:
	$(RM) shell
