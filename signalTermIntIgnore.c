#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void sigint_handler(int x)
{
    printf("sigint_handler");
}

int main()
{
    FILE* f = fopen("/home/cpp/pid", "w");
    fprintf(f, "%d\n", getpid());
    fclose(f);

    struct sigaction sigintNewAction, sigintOldAction;
    sigintNewAction.sa_handler = SIG_IGN;
    sigemptyset(&sigintNewAction.sa_mask);
    sigintNewAction.sa_flags = 0;
    sigaction(SIGINT, &sigintNewAction, &sigintOldAction);

    struct sigaction sigtermNewAction, sigtermOldAction;
    sigtermNewAction.sa_handler = SIG_IGN;
    sigemptyset(&sigtermNewAction.sa_mask);
    sigtermNewAction.sa_flags = 0;
    sigaction(SIGTERM, &sigtermNewAction, &sigtermOldAction);

    while(1)
    {
        printf("In the loop \n");
        sleep(100);
    }
}
