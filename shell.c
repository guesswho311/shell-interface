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
#define PROMPT_COLOR "\x1B[32m"
int run_commands(char * commands[]);
int run_built_in_commands(char * commands[]);
void cd(char * commands[]);

int main() {
  int cont = 1;
  char input[BUFFER_LENGTH]; //words inputted by user
  char * tokens[10];
  size_t n = 0;

  char *token; //stores one word from the input at a time
  const char s[2] = " ";

  while(cont == 1) {
    n = 0;
    printf("%sprompt$ ", PROMPT_COLOR);
    scanf("%[^\n]%*c",input);

    token = strtok(input, s);

    while(token != NULL) {
      tokens[n++] = token;
      if (strncmp(token, "ls", strlen("ls")) == 0 || strncmp(token, "dir", strlen("dir")) == 0 || strncmp(token, "vdir", strlen("vdir")) == 0 || strncmp(token, "grep", strlen("grep")) == 0) {
        tokens[n++] = "--color=auto";
      }
      token = strtok(NULL, s);
    }
    tokens[n++] = token;
    cont = run_commands(tokens); //run_commands returns an int, if 0 is returned that means the user would like to exit
  }
  printf("Exiting!\n");
}

//takes in a array of tokens. Parses through and runs the commands
int run_commands(char * commands[]) {
  pid_t pid;
  int status;
  int cont = 1;
  if (strncmp(commands[0],"exit", strlen("exit")) == 0 || strncmp(commands[0],"cd", strlen("cd")) == 0) {
    cont = run_built_in_commands(commands);
  }
  else {
    pid = fork();
    if (pid >= 0) {//fork was successful
      if (pid == 0) {
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
    else {
      printf("fork failed\n");
    }
  }
  return cont;
}

//A builtin function was found, this runs exit or if its 'cd' it will pass off the commands to the cd function
int run_built_in_commands(char * commands[]) {
  if (strncmp(commands[0],"exit", strlen("exit")) == 0) {
    return 0;
  }
  else {
    cd(commands);
    return 1;
  }
}

//Runs the cd command
void cd(char * commands[]) {
  if (commands[1] == '\0') {
      char * home = getenv("HOME");
      chdir(home);
      setenv("PWD", home, 1);
    }
    else if (strncmp(commands[1], "..", strlen("..")) == 0) {
      char * pwd = getenv("PWD");
      char * parent_location = strrchr(pwd, '/');
      *parent_location = '\0';
      chdir(pwd);
      setenv("PWD", pwd , 1);
    }
    else if(strncmp(commands[1],".", strlen(".")) == 0) {
      //do nothing
    }
    else {
      int status;
      char * pwd = getenv("PWD");
      char * new_directory = strcat(strcat(pwd, "/"), commands[1]);
      status = chdir(new_directory);

      if (status == 0) {
        setenv("PWD", new_directory, 1);
      } else {
        printf("Error, No such file or directory: %s\n", new_directory);
      }
    }
}
