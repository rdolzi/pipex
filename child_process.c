/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 02:27:48 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/24 02:49:17 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// char	**get_cmd(char **argv, int pos)
// {
// 	return (ft_split(argv[pos], ' '));
// }


char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*temp;
	char	*path;
	char	**base;
	
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	base = ft_split((env[i] + 5), ':');
	i = -1;
	while (base[++i])
	{
		temp = ft_strjoin(base[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, X_OK) == 0)
		{
			free_matrix(base);
			return (path);
		}
	}
	free(path);
	free_matrix(base);
	return (NULL);
}

void	ft_dup2(int *fd, int arg)
{
	if(dup2(*fd, arg) == -1)
			exit(1); // gestione errore
	close(*fd);
}

void	ft_execve(char *str, char **env, int *out_fd)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(str, ' ');
	path = get_path(cmd[0], env);
	ft_dup2(out_fd, STDOUT_FILENO); 
	if (!path)
	{
		free_matrix(cmd);
		perror("Path Error");
		exit(20); 
	}
	if (execve(path, cmd, env) == -1)
	{
		perror("Command Error");
		exit(30);
	}
}

void	child_process(char *str, char **env, int *fileout)
{
	pid_t	pid;
	int		fd[2];
	(void)fileout;
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		close(*fileout);
		ft_execve(str, env, &fd[1]); //chiude fd[1]
	}
	else
	{
		close(fd[1]);
		ft_dup2(&fd[0],STDIN_FILENO); //chiude fd[0]
		waitpid(pid, NULL, 0);
	}
}

