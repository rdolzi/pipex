
#include <stdio.h>

int main(int argc, char **argv, char**env)
{
    int i = -1;
    while (argv[++i])
        printf("%s\n", argv[i]);
    i = -1;
    while (env[++i])
        printf("%s\n", env[i]);
}
