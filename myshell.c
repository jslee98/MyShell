#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "history.h"
#include "myshell.h"

int executeInternalCommand(char **args)
{
  pid_t procId, waitingProcId;
  int status;

  procId = fork();
  if (procId == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("ERROR");
    }
    exit(1);
  } else if (procId < 0) {
    // Error forking
    perror("ERROR");
  } else {
    // Parent process
    do {
      waitingProcId = waitpid(procId, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int executeCommand(char **args) {
  // Who command
  if(strcmp(args[0], "who") == 0){
    if(args[1] == NULL) {
      printf("You must specify a subject (you or me)\n");
    } else if (strcmp(args[1], "me") == 0) {
      printf("You are the best programmer in the world!\n");
    } else if (strcmp(args[1], "you") == 0) {
      printf("I am the best computer in the world!\n");
    } else {
      printf ("You must specify a subject (you or me)\n");
    }
    return 1;
    // Exit Command
  } else if (strcmp(args[0], "exit") == 0) {
    return 0;
    // History Command
  } else if (strcmp(args[0], "history") == 0){
    printHistory();
    return 1;
    // Help Command
  } else if (strcmp(args[0], "help") == 0) {
    printf("Type who me or who you to use the custom shell, or use any other command to use the built in shell. 'exit' to quit.\n");
    return 1;
    // !! Command
  } else if (strcmp(args[0], "!!") == 0) {
    return executeNthCommand(1);
    // CD Command
  } else if (strcmp(args[0], "cd") == 0) {
    chdir(args[1]);
    printf("Changed working directory to: %s\n", args[1]);
    return 1;
    // !N Command
  } else if (args[0][0] == '!') {
    int n = args[0][1] - 48;
    return executeNthCommand(n);
  } else {
    return executeInternalCommand(args);
  }
}

char **parseCommand(char *command) {
  char **argArray  = NULL;
  char *currentArg = strtok(command, " \n");
  int argCount = 0;

  // Ensure currentArg exists, reallocate space and store in array
  while (currentArg != NULL) {
    argCount++;
    argArray = realloc(argArray, sizeof (char*) * argCount);
    argArray[argCount-1] = currentArg;
    currentArg = strtok(NULL, " \n");
  }

  // After storing commands, set last arg to NULL to indicate end of argument
  argCount++;
  argArray = realloc(argArray, sizeof (char*) * argCount);
  argArray[argCount - 1] = NULL;

  return argArray;
}

char *readCommand(void)
{
  char *line = NULL;
  ssize_t bufsize;
  getline(&line, &bufsize, stdin);
  return line;
}

void shellLoop(void) {
  char *command;
  char histCommand[100];
  char **args;
  int status = 0;

  do {
    printf("JL> ");
    command = readCommand();
    strcpy(histCommand, command);
    args = parseCommand(command);
    status = executeCommand(args);
    if(histCommand[0] != '!'){
      writeToHistory(histCommand);
    }
    free(command);
    free(args);
  } while (status);
}
