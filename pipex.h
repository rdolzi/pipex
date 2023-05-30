/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 16:03:18 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/30 16:03:20 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "Libft/libft.h"
# include <stdio.h>
//EXIT_SUCCESS
# include <stdlib.h>
//WRITE - PIPE - DUP2 - EXECVE - ACCESS
# include <unistd.h>
//OPEN
# include <fcntl.h>
//STRERROR
# include <string.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_setup
{
	int	i;
	int	filein;
	int	fileout;
	int	is_here_doc;
}	t_setup;

char	**get_cmd(char **argv, int pos);
void	child_process(char *str, char **env);
void	ft_execve(char *str, char **env);
void	ft_dup2(int *fd, int arg);
int		ft_here_doc(int *filein, char *limiter);
char	*get_path(char *cmd, char **env);
void	free_matrix(char **matrix);

//GNL
char	*get_next_line(int fd);
char	*ft_strjoin_gnl(char *s1, char *s2);
int		ft_strchr_gnl(char *s1, char c);
size_t	ft_strlen_gnl(char *str);
void	*ft_free(char *s1, char *s2);
size_t	ft_is_newline(char c);

#endif