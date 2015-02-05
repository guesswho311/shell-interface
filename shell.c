/***********************************************************
 * Name of program: Project 1 - Shell
 * Author: Jonathan Quist, Cole Boudreau
 **********************************************************/
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#define BUFFER_LENGTH 80
int run_commands(char * commands[], size_t n);
int run_built_in_commands(char * commands[], size_t n);
int main() {
  int cont = 1;
  char input[BUFFER_LENGTH]; //words inputted by user
  char * tokens[10];
  size_t n = 0;

  char *token; //stores one word from the input at a time
  const char s[2] = " ";

  while(cont == 1) {
    printf("prompt$ ");
    scanf("%[^\n]%*c",input);

    token = strtok(input, s);

    while(token != NULL) {
      tokens[n++] = token;
      token = strtok(NULL, s);
    }
    tokens[n++] = token;
    cont = run_commands(tokens, n); //run_commands returns an int, if 0 is returned that means the user would like to exit
  }
  printf("Exiting!\n");
}

//takes in a array of tokens and the array's size. Parses through and runs the commands
int run_commands(char * commands[], size_t n) {
  pid_t pid;
  int status;
  int cont = 1;
  if (strncmp(commands[0],"exit", strlen("exit")) == 0 || strncmp(commands[0],"cd", strlen("cd")) == 0) {
    cont = run_built_in_commands(commands, n);
  }
  else {
    if ((pid = fork()) == 0) {
      int command_status;
      command_status = execvp(commands[0], commands);
      exit(command_status);
    }
    else {
      waitpid(-1, &status, 0);
      if (status != 0) {
        printf("Error: Command could not be executed\n");
      }
    }
  }
  return cont;
}

int run_built_in_commands(char * commands[], size_t n) {
  if (strncmp(commands[0],"exit", strlen("exit")) == 0) {
    return 0;
  } else {
    int j;
    char * pwd = getenv("PWD");
    char * parent_location = strrchr(pwd, '/');
    *parent_location = '\0';
    j = chdir(pwd);
    printf("%d\n\n", j);
    setenv("PWD", pwd , 1);
    printf("%s\n",getenv("PWD"));

    return 1;
  }
}
