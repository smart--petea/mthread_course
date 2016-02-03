#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

char* getCmd(char** cmds, int* size)
{
    if(*cmds == NULL || **cmds == '\0')
    {
        *size = 0;
        return NULL;
    }

    assert(*cmds);

    char* cmdsTail = strchr(*cmds, '|');
    int resize;

    if(cmdsTail == NULL)
    {
        cmdsTail = strchr(*cmds, '\0');
        resize = *size = cmdsTail - *cmds;
    } else
    {
        *size = cmdsTail - *cmds;
        resize = *size + 1;
    }

    char* x = malloc(*size);
    strncpy(x, *cmds, *size);
    x[*size] = '\0';
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
    char* cmd;
    int size;
    while((cmd = getCmd(&commands, &size)) != NULL)
    {
        printf("%s\n", cmd);
        //if(fork())
        //{
        //    //parent
        //    int size = commandsTail - commands;
        //    char cmd[size];
        //    strncpy(cmd, commands, size);
        //    cmd[size] = '\n';
        //    printf("%s\n", cmd);
        //    break;
        //} else
        //{
        //    commands = commandsTail + 1;
        //    //child
        //}
    }

    return 0;
}
