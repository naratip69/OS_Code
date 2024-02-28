#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#define SNAME "mysem"

int main(int argc, char **argv)
{
	int num_agents = 2;
	if (argc > 1)
		num_agents = atoi(argv[1]);
	printf("Starting a call center with %d agents.\n", num_agents);

	sem_t *semaphore = sem_open(SNAME, O_CREAT, 0600, num_agents);

	int semval;
	while (1)
	{

		sem_getvalue(semaphore, &semval);

		printf("There are %d agents available now.\n", semval);
		sleep(3);
	}
}
