#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    printf("Please enter UNIX command\n");
    return 0;
  }
  pid_t pid;
  char *av[255];

  for (int i = 0; i < argc; ++i)
  {
    av[i] = argv[i + 1];
  }
  av[argc] = NULL;
  pid = fork();
  if (pid < 0)
  {
    printf("Error : cannot fork\n");
    exit(1);
  }
  else if (pid == 0)
  {
    execvp(av[0], av);
  }
  else
  {
    wait(NULL);
    return (0);
  }
}