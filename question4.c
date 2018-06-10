#include <unistd.h>
#include <signal.h>

int i = 0;
void handler()
{
    i = 4;
    printf("hello\n");
}

main()
{
    signal(SIGALRM, handler);
    alarm(4);
    for(i = 0; i < 7; i++)
    {
        printf("hello %d ...\n", i);
        sleep(2);
    }
}
