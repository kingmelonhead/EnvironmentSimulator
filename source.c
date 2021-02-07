//main source file for assignment 1
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
extern char **environ;
void print_vars(){
	char **ptr;
	char *key;
	char tempStr[5000];
	const char delim[2] = "=";
	for (ptr = environ; *ptr != NULL; ptr++){
		strcpy(tempStr, *ptr);
		key = strtok(tempStr, delim);
		printf("%s=%s\n", key, getenv(key));
	}
}
void update_env(int argc, char **args, char **vars){
	int count = 0;
	int util_start;
	int oldenv = 0;
	int util = 0;
	int i;
	int c;
	char *key;
	char **ptr;
	char tempStr[5000];
	int size;
	char **arrcopy;
	const char delim[2] = "=";
	for (ptr = vars; *ptr != NULL; ptr++){
		//gets number of original environment variables
		oldenv++;
	}
	for (i = 1; i < argc; i++){
		if (strcmp(args[i], "utility") == 0){
			//loop ends if utility keyword is hit
			util = 1;
			util_start = i+1;
			break;
		}
		else {
			strcpy(tempStr, args[i]);
			key = strtok(tempStr, delim);
			if (getenv(key) == NULL){
				size = strlen(args[i]);
				printf("Passed strlen\n");
				arrcopy[count] = malloc(sizeof(char *)*size);
				printf("passed malloc\n");
				strcpy(arrcopy[count], args[i]);
				printf("passed copy\n");
				//gets number of new variables
				count++;

			}
			else {
				putenv(args[i]); // updates vars that do already exist
			}
		}
	}
	count = oldenv + count;
	for (i = 0; i < count; i++){

	}
	print_vars();

}

void replace_env(int argc, char **args){
	char **newenv;
	printf("passed malloc lol\n");
	int util = 0; 
	int util_start;
	int util_count;
	int i;
	int count;
	for (i=2; i<argc; i++){
		if (strcmp(args[i], "utility") == 0){
			printf("found utility\n");
			util = 1;
			util_start = i+1;
			break;
			
		}
		else {
			count++;
		}
	}
	newenv = malloc(sizeof(char *) * count);
	for (i = 0; i< count; i++){
		newenv[i] = args[i+2];
	}
	environ = newenv;
	printf("New environment:\n\n");
	print_vars();
	printf("\nUtility:\n");
	if (util == 1){
		if (argc == util_start){
			printf("No utility given\n");
		}
		else {
			for (i = util_start; i < argc; i++){
				system(args[i]);
			}
		}
	}

}
void run_utility(int argc, char **args){

}
void display_help(){
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
}
int main(int argc, char *argv[], char *envp[]){
	
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
				printf("Invalid option given. Program will print current environment and end.\n");
				print_vars();
				return 0;
		}

	}
	if (replace == 1){
		replace_env(argc, argv);
	}
	else {	
		update_env(argc, argv, envp);
	}


	return 0;
}	
