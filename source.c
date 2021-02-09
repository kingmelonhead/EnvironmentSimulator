/*******************************************************************************************
 *
 * 	Name: John Hackstadt
 *	Email: jhackstadt50@gmail.com - personal
 *		jeh5h7@umsystem.edu - school
 *	Title:	Assignment 1 (cs 4760)
 *	Description: Environment Variable Tool
 *
 ******************************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
extern int errno;
extern char **environ;
int check_var(char *str){
	/*
	 *	Function used to check if a string is in the form [something]=[something]
	 *	-indicates something in the form of an environment variable
	 */
	int len = strlen(str);
	char *ch;
	if (len < 3 ){
		return 0;
	}
	int index = len -1;
	char *temp = malloc(sizeof(char) * (len + 1));
	temp = strncpy(temp, str, index);
	ch = strchr(temp, '=');
	if (ch == NULL){
		return 0;
	}
	else {
		return 1;
	}
	
}
void print_vars(){
	/*
	 *	This function is used for the printing of the environment variables to stdout 
	 */
	char *key, *tempStr;
	int count = 0;
	int size;
	int i = 0;
	const char delim[2] = "=";
	while (environ[count] != NULL){
		count++; // gets the number of env vars
	}
	for (i=0; i<count; i++){
		size = strlen(environ[i]);
		tempStr = malloc(size * sizeof(char *));
		strcpy(tempStr, environ[i]);
		key = strtok(tempStr, delim);
		printf("%s=%s\n", key, getenv(key));
		free(tempStr);
	}
	printf("\n");
}

void run_utility(char *arg){
	/*
	 *	external function used to run utility passed to doenv provided the utility arg is provided
	 *	using a separate function as to not have duplicate code in my project as this functionality
	 *	will be needed for both update_env() and replace_env()
	 */
	printf("System Call:\n\n");
	if (system(arg) != 0){
		errno = 22;
		perror("Error");
	}
	else {
		printf("\n");
	}
}

int check_inenv(char *var){
	//	used to check if a variable already exists in the environment
	char *key, *tempStr;
	int len = strlen(var);
	tempStr = malloc(sizeof(char) * len);
	strcpy(tempStr, var);
	key = strtok(tempStr, "=");
	if (getenv(key) != NULL){
		free(tempStr);
		return 1;
	}
	free(tempStr);
	return 0;
}

int get_loc(char *var){
	//	used to find where in the environment a variable already exists
	char *key1, *key2, *tempStr1, *tempStr2;
	char **ptr;
	int envLen;
	int index = 0;
	int len = strlen(var);
	tempStr1 = malloc(sizeof(char) * len);
	strcpy(tempStr1, var);
	key1 = strtok(tempStr1, "=");
	for (ptr = environ; *ptr != NULL; ptr++){
		envLen = strlen(*ptr);
		tempStr2 = malloc(sizeof(char) * envLen);
		strcpy(tempStr2, *ptr);
		key2 = strtok(tempStr2, "=");
		if (strcmp(key1, key2) == 0){
			free(tempStr1);
			free(tempStr2);
			return index;
		}
		index++;
	}
}



void update_env(int argc, char **args){
	/*
	 *	This function is used when user would like to add to / update the 
	 *	variables that already exist. In otherwords, if the variable is
	 *	already defined in the environment, it is updated. If not, it is 
	 *	added to the environment.
	 */
	char **newenv, **ptr;
	int total, i, len, loc;
	int old = 0;
	int index = 0;
	for (ptr = environ; *ptr != NULL; ptr++){
		old++;
	}
	total = old+argc;
	newenv = malloc(sizeof(char *) * total);
	for (ptr = environ; *ptr != NULL; ptr++){  // copies old environ to newenv
		len = strlen(*ptr);
		newenv[index] = malloc(sizeof(char) * len);
		strcpy(newenv[index], *ptr);
		index++;
	}
	newenv[index] = NULL;
	environ = newenv;
	for (i=1; i<argc; i++){
		if (check_var(args[i]) == 1 ){
			if (check_inenv(args[i]) == 1){
				loc = get_loc(args[i]); // if var already exists in environment then update environment
				free(newenv[loc]);
				len = strlen(args[i]);
				newenv[loc] = malloc(sizeof(char) * len);
				strcpy(newenv[loc], args[i]);
			}
			else {
				len = strlen(args[i]);	// if not then make a home for it in the new environment
				newenv[index] = malloc(sizeof(char) * len);
				strcpy(newenv[index], args[i]);
				index++;
				newenv[index] = NULL;
			}
		}
		else {
			//printf("Environment before running command: \n\n");
			//print_vars();
			run_utility(args[i]);
		}
	}
}

void replace_env(int argc, char **args){
	/*	
	 *	This function is called when the user wants to completely replace their whole environment
	 *	In other words, the entire existing environment is ignored and I make a new one and point
	 *	environ towards it
	 */
	char **newenv;
	int i, loc, len;
	int index = 0;
	newenv = malloc(sizeof(char *) * (argc + 1));
	environ = newenv;
	newenv[index] = NULL;
	for (i=2; i<argc; i++){
		if (check_var(args[i]) == 1){
			if (check_inenv(args[i]) == 0){
				newenv[index] = malloc(sizeof(char) * strlen(args[i])); // if not already in env then add it
				strcpy(newenv[index], args[i]);
				index++;
				newenv[index] = NULL;
			}
			else {
				loc = get_loc(args[i]);
				free(newenv[loc]);
				len = strlen(args[i]);
				newenv[loc] = malloc(sizeof(char) * len);
				strcpy(newenv[loc], args[i]);
			}
		}
		else {	
			//printf("Environment before running command:\n\n");
			//print_vars();
			run_utility(args[i]);
		}
	}
}

void display_help(){
	/*
	 *	function that gets called when ./doenv -h ... is called 
	 *	just displays a screen that resembles a man page for easier use of my program
	 */
	system("clear");
	printf("HELP MENU:\n\n");
	printf("Note: Call the program with no arguments to print out the current environment variables to the console.\n\n");
	printf("Usage:	./doenv [-i (optional)] [NAME=VALUE] ... [NAME=VALUE] .... [COMMAND] ... [COMMAND]\n\n");
	printf("   or:	./doenv -h \n\n");
	printf("[-h]	This option puts the program in help mode. All that is done is a menu (this one) gets displayed, then program halts.\n\n");
	printf("[-i]	This option is used to 'ignore' current environment variables, and replace them with the [NAME=VALUE] pairs\n");
	printf("	that are passed as arguments to the program.\n\n");
	printf("	Call the program without the -i option in order to modify the existing environment instead of replacing it\n");
	printf("	like would be done with the -i option. If a variable already exists (ex. PATH) then the value part of the\n");
	printf(" 	[NAME=VALUE] pair will be updated. If the NAME doesn't exist, then the variable will be added to the environ.\n\n");
	printf("	If an argument is encountered that is not in name=value pair form, it will be assumed that it is a system call and will be handled as so\n\n");
	printf("	Note: System calls and varables can be entered in any order, so the same call could be tested under multiple environ states if user wanted.\n\n");
	printf("More info is also in the README. Info surrounding how various components of the program function.\n\n");
}
int main(int argc, char *argv[]){
	/*
	 *	Main Function
	 */
	int opt;
	int replace = 0;
	printf("**************  	 environment tool 	 ********************************\n\n");
	if (argc == 1){
		print_vars();
		printf("\nCall this program with ./doenv [-i (optional)] [name=value] ... [command]\n");
		printf("To either modify or replace environment variables and run commands in this environment.\n");
		return 0;
	}
	
	while ((opt = getopt(argc, argv, "hi:")) != -1 ) {
		switch (opt) {
			case 'h':
				display_help();
				return 0;
				break;
			case 'i':
				printf("-i option recieved. Entire environment will be replaced\n");
				replace = 1;
				break;
			default:
				printf("Program will now print current environment and end.\n");
				print_vars();
				return 0;
		}

	}
	if (replace == 1){
		replace_env(argc, argv);
	}
	else {
		update_env(argc, argv);
	}
	printf("Final Environment State:\n\n");
	print_vars();
	return 0;
}	
