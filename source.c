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
extern char **environ;

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
}

void run_utility(int argc, char **args, int start){
	/*
	 *	externam function used to run utility passed to doenv provided the utility arg is provided
	 *	using a separate function as to not have duplicate code in my project as this functionality
	 *	will be needed for both update_env() and replace_env()
	 */
	int i;
	if (argc == start){
		printf("Utility was passed as an argument but no commands were provided after it\n");
		printf("Printing current environment instead:\n");
		print_vars();
	}
	else {
		for (i=start; i<argc; i++){
			system(args[i]);
		}
	}
}

void update_env(int argc, char **args){
	/*
	 *	This function is used when user would like to add to / update the 
	 *	variables that already exist. In otherwords, if the variable is
	 *	already defined in the environment, it is updated. If not, it is 
	 *	added to the environment.
	 */
	int i, util, util_start, size, total;
	char **newenv, **ptr;
	char *key, *tempStr;
	const char delim[2] = "=";
	int new_index = 0;	// used as an index counter for iterating through the newvars array
	int oldenv = 0;		// counts the # of vars in origonal environment
	int newvars[argc];	// array used to hold the indexes of new variables that need to be appended to the environment
	int count = 0; //used to count the number of arguments that represent name=value pairs
	for (ptr=environ; *ptr != NULL; ptr++){
		oldenv++;
	}
	for (i=1; i<argc; i++){
		if (strcmp(args[i], "utility") == 0){
			util_start = i + 1;
			util = 1;
			break;
		}
		else {
			size = strlen(args[i]);
			tempStr = (char *)malloc(sizeof(char) * (size + 1));
			strcpy(tempStr, args[i]);
			key = strtok(tempStr, delim);
			if (getenv(key) == NULL){
				count++; // count increases becasue its a new variable
				newvars[new_index] = i;
				new_index++;
			}
			else {
				putenv(args[i]); //anything not new gets updated
			}
			free(tempStr);
		}
	}
	total = count + oldenv;
	newenv = malloc(sizeof(char *) * (total + 1));
	for (i = 0; i<oldenv; i++){			//coppies existing environment into our new environment
		size = strlen(environ[i]);
		newenv[i] = (char *)malloc(sizeof(char) * (size+1));
		newenv[i] = environ[i];
	}
	count = 0;
	for (i = oldenv; i< total; i++){		//adds new variables to end of the environment
		size = strlen(args[newvars[count]]);
		newenv[i] = (char *)malloc(sizeof(char) * (size + 1));
		newenv[i] = args[newvars[count]];
		count++;
	}
	newenv[total] = NULL;			//makes final element in the pointer null so that print function works
	environ = newenv;			//sets environment to the new pointer
	if (util == 1){
		run_utility(argc, args, util_start);
	}
	else {
		printf("New environment:\n");
		print_vars();
	}
}

void replace_env(int argc, char **args){
	/*	
	 *	This function is called when the user wants to completely replace their whole environment
	 *	In other words, the entire existing environment is ignored and I make a new one and point
	 *	environ towards it
	 */
	char **newenv;
	int util = 0;  // boolean
	int count = 0;
	int util_start, i;
	for (i=2; i<argc; i++){
		if (strcmp(args[i], "utility") == 0){
			util = 1;
			util_start = i+1;
			break;
			
		}
		else {
			count++;
		}
	}
	newenv = malloc(sizeof(char *) * (count + 1));
	for (i = 0; i< count; i++){
		newenv[i] = malloc(sizeof(char *) * (strlen(args[i+2]) + 1));
		newenv[i] = args[i+2];
	}
	newenv[count] = NULL;
	environ = newenv;
	if (util == 1){
		run_utility(argc, args, util_start);
	}
	else {
		printf("New Environment:\n\n");
		print_vars();
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
	printf("Usage:	./doenv [-i (optional)] [NAME=VALUE] ... [NAME=VALUE] [utility (optional)] [COMMAND] ... [COMMAND]\n\n");
	printf("   or:	./doenv -h \n\n");
	printf("[-h]	This option puts the program in help mode. All that is done is a menu (this one) gets displayed, then program halts.\n\n");
	printf("[-i]	This option is used to 'ignore' current environment variables, and replace them with the [NAME=VALUE] pairs\n");
	printf("	that are passed as arguments to the program.\n\n");
	printf("	Call the program without the -i option in order to modify the existing environment instead of replacing it\n");
	printf("	like would be done with the -i option. If a variable already exists (ex. PATH) then the value part of the\n");
	printf(" 	[NAME=VALUE] pair will be updated. If the NAME doesn't exist, then the variable will be added to the environ.\n\n");
	printf("[utility]	if this argument is encountered, any argument provided after it will be ran under the modified/new environment.\n\n");
	printf("	Call the program without this option and the environment will simply be printed to the console (stdout).\n\n");
	printf("More info is also in the README. Info surrounding how various components of the program function.\n\n");
}
int main(int argc, char *argv[]){

	/*
	 *	Main Function
	 */

	system("clear"); // clear the console prior to running, maximizes screen real estate

	int opt;
	int replace = 0;
	
	if (argc == 1){
		print_vars();
		printf("\nCall this program with ./doenv [-i (optional)] [name=value] ... [utility (optional)] [command]\n");
		printf("To either modify or replace environment variables and run commands in this environment.\n");
		return 0;
	}
	
	while ((opt = getopt(argc, argv, "hi:")) != -1 ) {
		switch (opt) {
			case 'h':
				display_help();
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
	return 0;
}	
