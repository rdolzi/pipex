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

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

void	free_matrix(char **matrix);
char	**get_cmd(char **argv, int pos);
void	child_process(char *str, char **env, int *fileout);
//void	setup_files(int argc, char **argv, t_file *file);
//void	print_process(t_file *file);
void	ft_execve(char *str, char **env, int *out_fd);
void	ft_dup2(int *fd, int arg);

//GNL
char	*get_next_line(int fd);
char	*ft_strjoin_gnl(char *s1, char *s2);
int		ft_strchr_gnl(char *s1, char c);
size_t	ft_strlen_gnl(char *str);
void	*ft_free(char *s1, char *s2);
size_t	ft_is_newline(char c);

int		ft_here_doc(int *filein, char *limiter);
#endif