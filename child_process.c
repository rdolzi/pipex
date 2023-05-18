/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 02:27:48 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/18 04:45:58 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	set_fd(t_file *file)
{
	// leggere da STDIN -> passo il char **testo per execve
	if (file->is_bonus && file->is_heredoc)
	{

	}
}

char	**get_cmd(char **argv, int pos)
{
	return (ft_split(argv[pos], ' '));
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*temp;
	char	*path;
	char	**base;
	
	i = 0;
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	base = ft_split((env[i]+ 5), ':');
	i = -1;
	while (base[++i])
	{
		temp = ft_strjoin(base[i], "/");
		path = ft_strjoin(temp, "/");
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

// fd[0] - read
// fd[1] - write
void	setup_files(int argc, char **argv, t_file *file)
{

	if (pipe(file->fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (argv[1] == "here_doc")
	{
		file->filein = -2;
		file->fileout = open(argv[argc - 1], O_WRONLY, O_CREAT | O_APPEND, 0777);
		file->is_heredoc = 1;
	}
	else
	{
		file->filein = open(argv[1], O_RDONLY);  //se non esiste file di input?? "zsh: no such file or directory: in3.txt"
		file->fileout = open(argv[argc - 1], O_WRONLY, O_CREAT, 0777);
		file->is_heredoc = 0;
	}
	if (file->filein == -1 || file->fileout == -1 )
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}

void	child_process(char **argv, int pos, char **env, t_file *file)
{
	pid_t	pid;
	pid = fork();
	if (fork() == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		file->cmd = get_cmd(argv, pos);
		file->path = get_path(file->cmd[0], env);
		if (!file->path)
		{
			free_matrix(file->cmd);
			exit(5); // gestire messaggio errore "zsh: command not found: ciao" // check chiusura fd
		}
		set_fd(file);
		if(execve(file->path, file->cmd, env) == -1)  // or NULL ??
		{	
			perror("execve");
			exit (EXIT_FAILURE + 4);
		}
	}
	else
		waitpid(pid, NULL, 0);
}
