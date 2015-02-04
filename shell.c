/***********************************************************
 * Name of program: Project 1 - Shell
 * Author: Jonathan Quist, Cole Boudreau
 **********************************************************/
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#define BUFFER_LENGTH 80
void run_commands(char * commands[], size_t n);
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
    if (strncmp(input,"exit", strlen("exit")) == 0){
      cont = 0;
    }
    else {
      token = strtok(input, s);

      while(token != NULL) {
        tokens[n++] = token;
        token = strtok(NULL, s);
      }
      run_commands(tokens, n);
    }
  }
  printf("Exiting!\n");
}

//takes in a array of tokens and the array's size. Parses through and runs the commands
void run_commands(char * commands[], size_t n) {
  pid_t pid;
  size_t i;
  for (i = 0; i != n; ++i)
  {
    printf("%s\n", commands[i]);
  }
  //pid = fork();
//        if (pid >= 0) {
//          printf("\n%s\n", token);
//        token = strtok(NULL, s);

//        } else {
//          printf("Error: command not be executed.");
//        }
}
