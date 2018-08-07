#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "history.h"
#include "myshell.h"

void printHistory(void) {
  // Open file
  FILE *f;
  char s[1000];
  f = fopen("history.txt", "r");

  // Print each line of file
  if(f) {
    while(fgets(s, 1000, f) != NULL) {
      printf("%s", s);
    }
    fclose(f);
  } else {
    printf("No history found\n");
  }
}


int executeNthCommand(int n) {
  //printf("Executing %dth command...\n", n);
  // Opening file
  FILE *f;
  f = fopen("history.txt", "r");
  char **cmdArray  = (char **)malloc(sizeof(char *));
  char currentCmd[1000];
  int cmdCount = 0;

  // Loop through history and put it into array
  if(f) {
    while(fgets(currentCmd, 1000, f) != NULL) {
      cmdCount++;
      cmdArray = realloc(cmdArray, sizeof (char*) * cmdCount);
      cmdArray[cmdCount - 1] = strdup(currentCmd);
    }
    fclose(f);
  } else {
    printf("No history found\n");
  }
  // for(int i = 0; i < cmdCount; i++) {
  //   printf("Command[%d]: %s\n", i, cmdArray[i]);
  // }
  //printf("Call: Command[%d] = %s\n", cmdCount - n, cmdArray[cmdCount - n]);

  // Execute selected history
  char **args = parseCommand(cmdArray[cmdCount - n]);
  return executeCommand(args);
}

void writeToHistory(char* command) {
  FILE *file = fopen("history.txt", "a");
  int response = fputs(command, file);
  response = fclose(file);
  return;
}
