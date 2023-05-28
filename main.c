/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:51:25 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/28 14:22:07 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// ./pipex file1 cmd1 cmd2 file2 > argc=5 (cmds=2)   
// int main(int argc, char **argv, char **env)
// {
// 	int		filein;
// 	int		fileout; 

// 	if (argc != 5)
// 	{
// 		write(2, &"Error\n", 6);
// 		exit (EXIT_FAILURE);
// 	}
// 	filein = open(argv[1], O_RDONLY, 0777);
// 	if (filein == -1)
// 	{
// 		perror("Open error");
// 		exit (EXIT_FAILURE);
// 	}
// 	if (dup2(filein, STDIN_FILENO) == -1)
// 		exit(1);
// 	close(filein);
// 	fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
// 	if (fileout == -1)
// 	{
// 		close(filein);
// 		perror("Open error");
// 		exit (EXIT_FAILURE);
// 	}
// 	child_process(argv[2], env, &fileout);
// 	if (dup2(fileout, STDOUT_FILENO) == -1)
// 		exit(1);
// 	close(fileout);
// 	ft_execve(argv[3], env);
// }

// PROBLEMA: Path error non chiude fd
// int main(int argc, char **argv, char **env)
// {
// 	int i;
// 	int filein;
// 	int fileout;
// 	int is_here_doc;

// 	is_here_doc = !ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]));
// 	if (argc < 5 || (is_here_doc && argc != 6))
// 	{
// 		write(2, &"Error\n", 6);
// 		exit(1);
// 	}
// 	if (is_here_doc) // caso here_doc
// 	{
// 		i = 3;
// 		filein = 0; // ft_here_doc(&filein); crea filein
// 		fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
// 		if (fileout == -1)
// 		{
// 			close(filein);
// 			perror("Open error");
// 			exit(3);
// 		}
// 	}
// 	else
// 	{
// 		i = 2;
// 		filein = open(argv[1], O_RDONLY, 0777);
// 		if (filein == -1)
// 		{
// 			perror("Open error");
// 			exit(2);
// 		}
// 		fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
// 		if (fileout == -1)
// 		{
// 			close(filein);
// 			perror("Open error");
// 			exit(3);
// 		}
// 	}
// 	ft_dup2(&filein, STDIN_FILENO);
// 	while (i < argc - 2)
// 		child_process(argv[i++], env, &fileout);
// 	// if (is_here_doc)
// 	// unlink();
// 	int test = fork();
// 	if (test == 0)
// 	{
// 		ft_dup2(&fileout, STDOUT_FILENO);
// 		ft_execve(argv[i], env);
// 	}
// 	waitpid(test, NULL, 0);
// }

void ft_setup(int argc, char **argv, t_setup *setup)
{
	setup->is_here_doc = !ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]));
	if (argc < 5 || (setup->is_here_doc && argc != 6))
	{
		write(2, &"Error\n", 6);
		exit(1);
	}
	if (setup->is_here_doc) // caso here_doc
	{
		setup->i = 3;
		setup->filein = 0; // ft_here_doc(&filein); crea filein
		setup->fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
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
		setup->fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
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
int main(int argc, char **argv, char **env)
{
	t_setup	setup;

	ft_setup(argc, argv, &setup);
	while (setup.i < argc - 2)
		child_process(argv[setup.i++], env, &setup.fileout);
	// if (is_here_doc)
	// unlink();
	int test = fork();
	if (test == 0)
	{
		ft_dup2(&setup.fileout, STDOUT_FILENO);
		ft_execve(argv[setup.i], env);
	}
	waitpid(test, NULL, 0);
}
