#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builtins.h"

// List of all builtin commands
char* builtin_commands[] = {"cd", "help", "exit"};

/*
* Returns the number of builtin commands
*/
int ush_num_builtins() {
	return sizeof(builtin_commands) / sizeof(char*);
}

/*
* Returns the builtin commands
*/
char** ush_get_builtins() {
	return builtin_commands;
}

/*
* List of function pointers for builtin commands
*/
int (*builtin_funcs[]) (char**) = {
	&ush_cd, 
	&ush_help,
	&ush_exit
};

/*
* execute the builtin cd command
*/
int ush_cd(char** args) {
	if (args[1] == NULL) {
		fprintf(stderr, "ush: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("ush");
		}
	}
	return 1;
}

/*
* execute the builtin help command
*/
int ush_help(char** args) {
	int i;
	printf("Unnsteins Shell\n");
	printf("Type program names\n");
	printf("The following are built in\n");
	for(i = 0; i < ush_num_builtins(); i++) {
		printf("\t%s\n", ush_get_builtins()[i]);
	}
	return 1;
}

/*
* execute the builtin exit command
*/
int ush_exit(char** args) {
	return 0;
}





