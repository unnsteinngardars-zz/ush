#ifndef BUILTINS
#define BUILTINS

int ush_num_builtins();
char** ush_get_builtins();
extern int (*builtin_funcs[]) (char**);

int ush_cd(char** args);
int ush_help(char** args);
int ush_exit(char** args);

#endif
