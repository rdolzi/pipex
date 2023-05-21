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

//filein/fileout: -1 se errore open / -2 se non settato
//is_heredoc: -1 se non si tratta del bonus
typedef struct s_file
{
	int		idx;
	int		fd[2];
	int		filein;
	int		fileout;
	int		is_bonus;
	int		elements;
	int		is_heredoc;
	char	**cmd;
	char	*path;
}   t_file;

void	free_matrix(char **matrix);
char	**get_cmd(char **argv, int pos);
void	child_process(char **argv, int pos, char **env, t_file *file);
void	setup_files(int argc, char **argv, t_file *file);
void	print_process(t_file *file);
#endif