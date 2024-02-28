#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void routine(void *arg)
{
  printf("%s\n", ((char *)arg));
}

int main(int argc, char *argv[])
{
  pid_t id = fork();
  if (id < 0)
  {
    return 1;
  }
  else if (id == 0)
  {
    pthread_t t[2];
    pthread_create(t, NULL, *routine, "First thread from child process");
    pthread_create(t + 1, NULL, *routine, "Second thread from child process");
    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
  }
  else
  {
    pthread_t t[2];
    pthread_create(t, NULL, *routine, "First thread from parent process");
    pthread_create(t + 1, NULL, *routine, "Second thread from parent process");
    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
  }
  return 0;
}