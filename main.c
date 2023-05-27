/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:51:25 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/28 00:54:06 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// MANDATORY:
// Your program will be executed as follows:
//     ./pipex file1 cmd1 cmd2 file2
// It must take 4 arguments:
//     • file1 and file2 are file names.
//     • cmd1 and cmd2 are shell commands with their parameters.
// It must behave exactly the same as the shell command below:
// $> < file1 cmd1 | cmd2 > file2

// ESEMPIO:
// $> ./pipex infile "ls -l" "wc -l" outfile
// Should behave like: 
//     < infile ls -l | wc -l > outfile
// $> ./pipex infile "grep a1" "wc -w" outfile
// Should behave like:
//     < infile grep a1 | wc -w > outfile

// TODO:


//1. infile
// aprire il file:
// -se non esiste?
// -se non ha permessi in lettura?
// e3r10p5% < ciao.txt wc -c   (chmod 377 / 177 ciao.txt)  >  read (5) write(2) execute(1)   > 1.OWNER 2.GROUP 3.EVERYONE
// zsh: permission denied: ciao.txt

// 2. cmd1 & cmd2
// a. verificare che il comando esista:
// 	   >"ls -l", ma vale anche "ls -l -a -t"
//     >va scompattato separando il comando dalle opzioni, sapendo che:
// 			>> valore [0] rappresenta sempre il comando
// 			>> valori n successivi rappresentano le opzioni3
//     >check che access (path+cmd) sia ok altrimenti cmd_error
//     >non vanno gestiti i parametri non validi. ci pensa excve a ritornare un messaggio

// 3. outfile
//     >se non esiste viene creato
//     >se esiste viene sovrascritto

// ./pipex infile cmd1 cmd2 outfile

// ./pipex file1 cmd1 cmd2 file2 > argc=5 (cmds=2)   >inf RDONLY >outf 0_WRITE | O_CREAT
// ./pipex file1 cmd1 cmd2 ... cmdN file2 > argc = n (cmds=n-3)  >inf RDONLY >outf 0_WRITE | O_CREAT
// ./pipex here_doc LIMITER cmd cmd1 file > argc=6 >inf RDONLY >outf 0_WRITE | O_CREAT


// TODO:
// leaks atExit rimane in running?

//MAIN NORMALE SENZA STRUCT > OK!
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
//	close(fileout); ?? test this line
// 	ft_execve(argv[3], env);
// }

// ./pipex file1 cmd1 cmd2 file2 > argc=5 (cmds=2)   >inf RDONLY >outf 0_WRITE | O_CREAT
// ./pipex file1 cmd1 cmd2 ... cmdN file2 > argc = n (cmds=n-3)  >inf RDONLY >outf 0_WRITE | O_CREAT
// ./pipex here_doc LIMITER cmd cmd1 file > argc=6 >inf RDONLY >outf 0_WRITE | O_CREAT

//MAIN BONUS SENZA STRUCT
int main(int argc, char **argv, char **env)
{
	int		i;
	int		filein;
	int		fileout; 
	int		is_here_doc;

	is_here_doc = !ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]));
	if (argc < 5 || (is_here_doc && argc != 6))
	{
		write(2, &"Error\n", 6);
		exit (1);
	}
	if (is_here_doc)  // caso here_doc
	{
		i = 3;
		filein = 0; //ft_here_doc(&filein); crea filein
		fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (fileout == -1)
		{
			close(filein);
			perror("Open error");
			exit (3);
		}
	}
	else
	{
		i = 2;
		filein = open(argv[1], O_RDONLY, 0777);
		if (filein == -1)
		{
			perror("Open error");
			exit (2);
		}
		fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fileout == -1)
		{
			close(filein);
			perror("Open error");
			exit (3);
		}
	}
	if (dup2(filein, STDIN_FILENO) == -1)
		exit(1);
	close(filein);
	// ft_dup2(&filein, STDIN_FILENO); //chiuso filein // manca fileout
	while (i < argc - 2)
		child_process(argv[i++], env, &fileout);
	// if (is_here_doc)
	// unlink();
	if (dup2(fileout, STDOUT_FILENO) == -1)
		exit(1);
	close(fileout);
	ft_execve(argv[i], env);
}
