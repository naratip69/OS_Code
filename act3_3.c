#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
main()
{
    int i;
    int n;
    pid_t childpid; 
    n = 4;
    for (i = 0; i < n; ++i) {
        childpid = fork() ;
        if (childpid == 0) break;
        wait(0);
    }
    printf("This is process %ld with parent %ld\n",(long) getpid (), (long)
    getppid());
}