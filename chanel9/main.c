#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

char* getCmd(char** cmds)
{
    if(*cmds == NULL || **cmds == '\0')
    {
        return NULL;
    }

    assert(*cmds);

    char* cmdsTail = strchr(*cmds, '|');
    int resize;
    int size;

    if(cmdsTail == NULL)
    {
        cmdsTail = strchr(*cmds, '\0');
        resize = size = cmdsTail - *cmds;
    } else
    {
        size = cmdsTail - *cmds;
        resize = size + 1;
    }

    char* x = malloc(size);
    strncpy(x, *cmds, size);
    x[size] = '\0';
    *cmds = *cmds + resize;
    return x;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "[usage] %s commands \n", argv[0]);
        return 1;
    }

    char* commands = argv[1];
    char* cmdParent;
    char* cmdChild;
    cmdParent = getCmd(&commands);
    //parent
    printf("cmdParent = %s\n", cmdParent);

    while((cmdChild = getCmd(&commands)) != NULL)
    {
        if(fork())
        {
            break;
        } else
        {
            printf("cmdChild = %s\n", cmdChild);
            //child
        }
    }

    //free memory
    free(cmdParent);
    free(cmdChild);
    return 0;
}
