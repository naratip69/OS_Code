#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    int i;
    int n;
    int childpid;
    n = 4;
    for (i = 0; i < n; ++i)
    {
        childpid = fork();
        if (childpid > 0)
        {
            wait(4 - i);
            break;
        }
    }
    printf("This is process %ld with parent %ld\n", (long)getpid(), (long)getppid());
    wait(0);
}