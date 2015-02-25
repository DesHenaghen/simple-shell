/* ACE 4 */

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>

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

int main() {
	char directory[] = getHomeDir(); /*gets initial working directory*/
	printf("%s", get_input(directory));
	return 0;
}
