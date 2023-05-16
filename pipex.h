#ifndef PIPEX_H
# define PIPEX_H

#include "Libft/libft.h"
#include <stdio.h>
//EXIT_SUCCESS
#include <stdlib.h>
//WRITE - PIPE - DUP2 - EXECVE
#include <unistd.h>
//OPEN
#include <fcntl.h>

char	**get_cmd(char *argv, char **env);
#endif