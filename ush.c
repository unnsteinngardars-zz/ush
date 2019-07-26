/* Custom made shell built from https://brennan.io/2015/01/16/write-a-shell-in-c/ */

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "builtins.h"

/* CONSTANTS */
const char* SPLIT_LINE_DELIMETER = " \t\r\n\a"; 

void ush_run();
char* ush_read_line();
char** ush_split_line(char* line, const char* delim);
int ush_launch(char** args);
int ush_execute(char** args);

int main (int argc, char** argv) {

	// Load config files
		
	// Run command loop		
	ush_run();

	// Perform any cleanup
	
	return EXIT_SUCCESS;
}

/*
* Run the shell
*/
void ush_run() {
	char* line;
	char** args;
	int status;
	do {
		printf("ush> ");
		line = ush_read_line();
		args = ush_split_line(line, SPLIT_LINE_DELIMETER);
		status = ush_execute(args);
		free(line);
		free(args);
	} while(status);
}

/*
* Reads and returns a single line from stdin
*/
char* ush_read_line() {
	char* line = NULL;
	ssize_t bufsize = 0;
	getline(&line, &bufsize, stdin);
	return line;
}

/*
* Returns an array of tokens from the provided string split by the delimeter
*/
char** ush_split_line(char* line, const char* delim) {
	const int USH_TOK_BUFSIZE = 60;
	int bufsize = USH_TOK_BUFSIZE, position = 0;
	char** tokens = malloc(bufsize * sizeof(char*));
	char* token;

	if (!tokens) {
		fprintf(stderr, "ush: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, delim);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += USH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "ush: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, delim);
	}
	tokens[position] = NULL;
	return tokens;
}

/*
* Launch the given command that is not a builtin one
*/
int ush_launch(char** args) {
	pid_t pid, wpid;
	int status;
	pid = fork();

	if (pid == 0) {
		if (execvp(args[0], args) == -1) {
			perror("ush");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("ush");
	} else {
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
} 

/*
* execute command, builtin or not
*/
int ush_execute(char** args) {
	int i;
	char** builtins = ush_get_builtins();
	
	if (args[0] == NULL) {
		return 1;
	}
	for (i = 0; i < ush_num_builtins(); i++) {
		if (strcmp(args[0], builtins[i]) == 0) {
			return (*builtin_funcs[i])(args);
		}
	}
	return ush_launch(args);
}








