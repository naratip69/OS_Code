#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void routine(void *arg)
{
  printf("This is thread %d\n", *((int *)arg));
  free(arg);
}

int main(int argc, char *argv[])
{
  pthread_t t[10];
  for (int i = 0; i < 10; i++)
  {
    int *arg = malloc(sizeof(int));
    *arg = i + 1;
    pthread_create(t + i, NULL, *routine, arg);
  }
  for (int i = 0; i < 10; i++)
  {
    pthread_join(t[i], NULL);
  }
  return 0;
}