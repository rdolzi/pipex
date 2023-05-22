/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 02:27:48 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/22 02:07:59 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

// casi:
// 	1.is_here_doc && pos ==3(primo)
// 		>leggere da STDOUT
// 		>scrivere in fd[1] /
// 	2.is_here_doc && pos ==4(ultimo)
// 		>leggere in fd[0] -
// 		>scrivere in fileout ?
// 	3.!is_here_doc && pos ==2(primo)
// 		>leggere da filein
// 		>scrivere in fd[1] /
// 	4.!is_here_doc && pos == file->elements (ultimo)
// 		>leggere da fd[0] -
// 		>scrivere in fileout ?
// 	5.!is_here_doc && (pos > 2 && pos < file->elements) (n-esimo)
// 		>leggere da fd[0]
// 		>scrivere in fd[1] /
void	set_fd_bonus(t_file *file, int pos)
{
	if (file->is_heredoc && pos == 3)
	{
			printf("is doc && is child 3 && bonus\n");
			close(file->fd[0]);
			ft_dup2(&file->fd[1], STDIN_FILENO);
	}
	else if (file->is_heredoc && pos == 4)
	{
			printf("is doc && is child 4 && bonus\n");
			close(file->fd[1]);
			close(file->filein);
			ft_dup2(&file->fd[0], STDOUT_FILENO);
			ft_dup2(&file->fileout, STDIN_FILENO);
	}
	else if (!file->is_heredoc && pos == 2)
	{
			printf("is !doc && is child 2 && bonus\n");
			close(file->fd[0]);
			close(file->fileout);
			ft_dup2(&file->filein, STDIN_FILENO);
			ft_dup2(&file->fd[1], STDOUT_FILENO);
	}
	else if (!file->is_heredoc && pos == file->elements - 1)
	{
			printf("is !doc && is child = file->elements:%d\n", file->elements);
			close(file->fd[1]);
			close(file->filein);
			ft_dup2(&file->fd[0], STDIN_FILENO);
			ft_dup2(&file->fileout, STDOUT_FILENO);
	}
	else if (!file->is_heredoc && (pos > 2 && pos < file->elements))
	{
			printf("is !doc && is child >2 && < file->elements:%d\n", file->elements);
			close(file->filein);
			close(file->fileout);
			ft_dup2(&file->fd[0], STDIN_FILENO);
			ft_dup2(&file->fd[1], STDOUT_FILENO);
	}
}

// fd[0] - read
// fd[1] - write
void	set_fd(t_file *file, int pos)
{
	if (file->is_bonus)
		set_fd_bonus(file, pos);
	else if (!file->is_bonus && pos == 2)
	{
		printf("is child 2 && !bonus\n");
		close(file->fd[0]);
		close(file->fileout);
		ft_dup2(&file->filein, STDIN_FILENO);
		ft_dup2(&file->fd[1], STDOUT_FILENO);
	}
	else if (!file->is_bonus && pos == 3)
	{
		printf("is child 3 && !bonus\n");
		close(file->fd[1]);
		close(file->filein);
		ft_dup2(&file->fd[0], STDIN_FILENO);
		ft_dup2(&file->fileout, STDOUT_FILENO);
	}
}

void	set_bonus(int argc, char **argv, t_file *file)
{
	if (!ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])))
	{
		file->filein = -2;
		file->fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		file->idx = 2;
		file->is_heredoc = 1;
	}
	else
	{
		file->filein = open(argv[1], O_RDONLY);  //se non esiste file di input?? "zsh: no such file or directory: in3.txt"
		file->fileout = open(argv[argc - 1], O_WRONLY | O_CREAT, 0777);
		file->is_heredoc = 0;
	}
}

//V2 SEPARAZIONE BONUS & NON
void	setup_files(int argc, char **argv, t_file *file)
{
	if (pipe(file->fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (file->is_bonus)
		set_bonus(argc, argv, file);
	if (!file->is_bonus)
	{
		file->filein = open(argv[1], O_RDONLY);  //se non esiste file di input?? "zsh: no such file or directory: in3.txt"
		file->fileout = open(argv[argc - 1], O_WRONLY | O_CREAT, 0777);
		file->is_heredoc = 0;
	}
	if (file->filein == -1 || file->fileout == -1 )
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}

void	child_process(char **argv, char **env, t_file *file)
{
	pid_t pid;
	(void)env;
	(void)argv;
	// int fd[2];

	// if (pipe(file->fd) == -1)
	// {
	// 	exit(EXIT_FAILURE);
	// }
	printf("AGAIN%d\n", file->idx);
	pid = fork();
	if (pid == 0)
	{
		printf("Hello world!%d\n", file->idx);
		file->cmd = get_cmd(argv, file->idx);
		file->path = get_path(file->cmd[0], env);
		if (!file->path)
		{
			free_matrix(file->cmd);
			exit(5); // gestire messaggio errore "zsh: command not found: ciao" // check chiusura fd
		}
		set_fd(file, file->idx);
		printf(">>!%d\n", file->idx);
		if (execve(file->path, file->cmd, env) == -1) // or NULL ??
		{
			perror("execve");
			exit(EXIT_FAILURE + 4);
		}
		printf("\n--IN CHILD %d FILE--\n", file->idx);
		print_process(file);
		exit(0);
	}
	else
	{
		close(file->fd[1]);
		waitpid(pid, NULL, 0);
	}
}
