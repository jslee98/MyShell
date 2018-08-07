#ifndef MYSHELL_H_INCLUDED
#define MYSHELL_H_INCLUDED
#endif

int executeInternalCommand(char **args);
int executeCommand(char **args);
char **parseCommand(char *command);
char *readCommand(void);
void shellLoop(void);
