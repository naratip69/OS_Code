#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
  int run = 1;
  while (run)
  {
    printf("soeish >");
    char delim[] = " \t\n";
    char **tokens;
    char av[255];
    int token_status;
    fgets(av, 255, stdin);
    token_status = tokenize(av, delim, &tokens);
    if (token_status == -1)
    {
      printf("Error: Can't create tokens\n");
      return 1;
    }
    int id = fork();
    if (id < 0)
    {
      printf("Error: Can't fork\n");
      return 2;
    }
    else if (id == 0)
    {
      if (strcmp(tokens[0], "exit") == 0)
      {
        return 200;
      }
      int err = execvp(tokens[0], tokens);
      if (err == -1)
      {
        printf("Error: Command not found\n");
        return 404;
      }
    }
    else
    {
      int wait_status;
      wait(&wait_status);
      if (WIFEXITED(wait_status))
      {
        int status = WEXITSTATUS(wait_status);
        if (status == 200)
        {
          return 0;
        }
      }
    }
    /*
    After reading user input, do these steps
    1. use tokenize() function to get command
    2. fork a child process
    3. child use execvp() to run command
    4. parent call wait() until user enter "exit"
    */
  }
  return 0;
}
/*
Split input string into substrings (called tokens)
which are sequences of contiguous characters separated by any of the
characters
in the string of accepted delimiters and fill the tokens into an
array.
The last element of the array contains a NULL pointer.
Return number of tokens or -1 if not success.
Example:
 char delim[] = " \t\n";
 char **tokens;
 char string[256];
 int numtokens;
 int i;
 fgets(string, 256, stdin);
 numtokens = tokenize(string, delim, &tokens);
 for (i = 0; i < numtokens; i++) {
 printf("%d:%s\n", i, tokens[i]);
 }
*/
int tokenize(char *string, char *delimiters, char ***arrayOfTokens)
{
  char *token;
  int numtokens;
  int i;
  /* skip the beginning delimiters */
  string += strspn(string, delimiters);
  if ((token = malloc(strlen(string) + 1)) == NULL)
    return -1;
  /* count tokens */
  strcpy(token, string);
  numtokens = 0;
  if (strtok(token, delimiters) != NULL)
    for (numtokens = 1; strtok(NULL, delimiters) != NULL;
         numtokens++)
      ;
  /* create array of pointers to tokens */
  if ((*arrayOfTokens = malloc((numtokens + 1) * sizeof(char *))) ==
      NULL)
  {
    free(token);
    return -1;
  }
  /* fill pointers to tokens into the array */
  if (numtokens == 0)
    free(token);
  else
  {
    strcpy(token, string);
    (*arrayOfTokens)[0] = strtok(token, delimiters);
    for (i = 1; i < numtokens; i++)
      (*arrayOfTokens)[i] = strtok(NULL, delimiters);
    (*arrayOfTokens)[numtokens] = NULL;
  }
  return numtokens;
}