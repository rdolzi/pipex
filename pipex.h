#ifndef PIPEX_H
# define PIPEX_H

#include "Libft/libft.h"
#include <stdio.h>
//EXIT_SUCCESS
#include <stdlib.h>
//WRITE - PIPE - DUP2 - EXECVE - ACCESS
#include <unistd.h>
//OPEN
#include <fcntl.h>
//STRERROR
#include <string.h>

typedef struct s_setup
{
    int i;
    int filein;
    int fileout;
    int is_here_doc;
}   t_setup;

char	**get_cmd(char **argv, int pos);
void	child_process(char *str, char **env, int *fileout);
void	ft_execve(char *str, char **env);
void	ft_dup2(int *fd, int arg);
#endif