//main source file for assignment 1
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
extern char **environ;
void print_vars(){
	system("clear");
	char **ptr;
	char *key;
	char tempStr[10000];
	const char delim[2] = "=";
	for (ptr = environ; *ptr != NULL; ptr++){
		strcpy(tempStr, *ptr);
		key = strtok(tempStr, delim);
		printf("%s=%s\n", key, getenv(key));
	}
}
void update_env(){

}

void replace_env(){


}
int main(int argc, int *argv[]){
	print_vars();
	return 0;
}	
