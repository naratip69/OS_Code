#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

#define SNAME "mysem"

int rand_int(int n)
{
	// Generate random integer number between [1, n]
	int r = rand();
	return (r % n) + 1;
}

int main(int argc, char **argv)
{
	printf("Starting customer\n");

	sem_t *semaphore = sem_open(SNAME, 0);

	while (1)
	{
		// Customer will wait between 1-3 seconds before placing the next phone call
		int wait_time = rand_int(3);
		printf("Wait for %d minutes\n", wait_time);
		sleep(wait_time);
		printf("Next customer calls the call center, press 10 buttons, and listens to silly music.\n");
		time_t t0 = time(NULL);
		// Wait for an agent

		sem_wait(semaphore);

		time_t t = time(NULL) - t0;
		// An agent accepts the call, using it for 1-5 seconds.
		int call_time = rand_int(5);
		printf("An agent accepts the call after waiting for %ld minutes.  Talk, talk, talk for %d minutes.\n", t, call_time);
		sleep(call_time);
		// Customer hangs up the phone

		sem_post(semaphore);

		printf("Customer ends the call.\n");
	}
}
