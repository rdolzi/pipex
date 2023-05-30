/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:51:25 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/30 15:58:44 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_perror(char *str, int i, int *fd)
{
	perror(str);
	exit(i);
	if (fd)
		close(*fd);
}

int	ft_here_doc(int *filein, char *limiter)
{
	char	*str;

	*filein = open("temp.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
	if (*filein == -1)
		ft_perror("Open error", 3, filein);
	while ((ft_strncmp(str, limiter, ft_strlen(str) - 1)) || (
			ft_strlen(str) - 1) != ft_strlen(limiter))
	{
		write(1, &"pipe heredoc>", 13);
		str = get_next_line(0);
		if (write(*filein, str, ft_strlen(str)) == -1)
			ft_perror("Write error", 22, NULL);
		if (!str)
		{
			if (unlink("./temp.txt") != 0)
				ft_perror("unlink error", 21, NULL);
		}
		free(str);
	}
	close(*filein);
	*filein = open("temp.txt", O_RDONLY, 0777);
	return (*filein);
}

void	ft_setup(int argc, char **argv, t_setup *setup)
{
	if (setup->is_here_doc)
	{
		setup->i = 3;
		ft_here_doc(&setup->filein, argv[2]);
		setup->fileout = open(argv[
				argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (setup->fileout == -1)
			ft_perror("Open error", 3, &setup->filein);
	}
	else
	{
		setup->i = 2;
		setup->filein = open(argv[1], O_RDONLY, 0777);
		if (setup->filein == -1)
			ft_perror("Open error", 2, NULL);
		setup->fileout = open(argv[
				argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (setup->fileout == -1)
			ft_perror("Open error", 3, &setup->filein);
	}
	ft_dup2(&setup->filein, STDIN_FILENO);
}

void	check_cmd(int argc, char **argv, char **env, t_setup *setup)
{
	int		j;
	char	**cmd;
	char	*path;

	j = setup->i;
	while (j <= argc - 2)
	{
		cmd = ft_split(argv[j++], ' ');
		path = get_path(cmd[0], env);
		if (!path)
		{
			close(setup->fileout);
			close(setup->filein);
			free_matrix(cmd);
			perror("Path Error");
			exit(20);
		}
		free_matrix(cmd);
		free(path);
	}
}

//V2 WITH T_SETUP
int	main(int argc, char **argv, char **env)
{
	int		test;
	t_setup	setup;

	setup.is_here_doc = !ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]));
	if (argc < 5 || (setup.is_here_doc && argc != 6))
		exit(write(2, &"Error\n", 6));
	ft_setup(argc, argv, &setup);
	check_cmd(argc, argv, env, &setup);
	close(setup.filein);
	while (setup.i < argc - 2)
		child_process(argv[setup.i++], env);
	if (setup.is_here_doc)
		unlink("temp.txt");
	test = fork();
	if (test == 0)
	{
		ft_dup2(&setup.fileout, STDOUT_FILENO);
		ft_execve(argv[setup.i], env);
	}
	waitpid(test, NULL, 0);
	close(setup.fileout);
}
