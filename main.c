/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:51:25 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/30 02:04:30 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_here_doc(int *filein, char *limiter)
{
	char	*str;

	*filein = open("temp.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
	if (*filein == -1)
	{
		close(*filein);
		perror("Open error");
		exit (3);
	}
	while ((ft_strncmp(str, limiter, ft_strlen(str) - 1)) || (ft_strlen(str) - 1) != ft_strlen(limiter))
	{
		write(1, &"pipe heredoc>", 13);
		str = get_next_line(0);
		if (write(*filein, str, ft_strlen(str)) == -1)
		{
			perror("Write error");
			exit(22);
		}
		if (!str)
		{
			if (unlink("./temp.txt") != 0)
				perror("unlink error");
		}
		free(str);
	}
	close(*filein);
	*filein = open("temp.txt", O_RDONLY, 0777);
	return (*filein);
}

void	ft_setup(int argc, char **argv, t_setup *setup)
{
	setup->is_here_doc = !ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]));
	if (argc < 5 || (setup->is_here_doc && argc != 6))
	{
		write(2, &"Error\n", 6);
		exit(1);
	}
	if (setup->is_here_doc)
	{
		setup->i = 3;
		ft_here_doc(&setup->filein, argv[2]);
		setup->fileout = open(argv[
				argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (setup->fileout == -1)
		{
			close(setup->filein);
			perror("Open error");
			exit(3);
		}
	}
	else
	{
		setup->i = 2;
		setup->filein = open(argv[1], O_RDONLY, 0777);
		if (setup->filein == -1)
		{
			perror("Open error");
			exit(2);
		}
		setup->fileout = open(argv[
				argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (setup->fileout == -1)
		{
			close(setup->filein);
			perror("Open error");
			exit(3);
		}
	}
	ft_dup2(&setup->filein, STDIN_FILENO);
}

//V2 WITH T_SETUP
int	main(int argc, char **argv, char **env)
{
	int		j;
	t_setup	setup;
	char	**cmd;
	char	*path;

	ft_setup(argc, argv, &setup);
	j = setup.i;
	while (j <= argc - 2)
	{
		cmd = ft_split(argv[j++], ' ');
		path = get_path(cmd[0], env);
		if (!path)
		{
			close(setup.fileout);
			close(setup.filein);
			free_matrix(cmd);
			perror("Path Error");
			exit(20);
		}
		free_matrix(cmd);
		free(path);
	}
	close(setup.filein);
	while (setup.i < argc - 2)
		child_process(argv[setup.i++], env, &setup.fileout);
	if (setup.is_here_doc)
	{
		close(setup.filein);
		unlink("temp.txt");
	}
	close(setup.filein);
	int test = fork();
	if (test == 0)
	{
		ft_dup2(&setup.fileout, STDOUT_FILENO);
		ft_execve(argv[setup.i], env);
	}
	waitpid(test, NULL, 0);
	close(setup.fileout);
	close(setup.filein);
}
