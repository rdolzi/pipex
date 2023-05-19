/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:51:25 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/20 01:18:47 by rdolzi           ###   ########.fr       */
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
// nella struttura conservare STDIN/OUT ??
int main(int argc, char **argv, char **env)
{
	t_file	file;

	if (argc != 5) // se here_doc non in bonus?
	{
		write(2, &"Error\n", 6);
		return (EXIT_FAILURE  + 1);
	}
	file.argc = argc;
	file.is_bonus = 0;
	file.idx = 2;
	setup_files(argc, argv, &file);
	(void)env;
	while ((file.argc--) - 3 - file.is_heredoc > 0)
	{
		printf("file.argc:%d\n",file.argc);
		printf("file.is_heredoc:%d\n",file.is_heredoc);
		printf("argc - 3 - file.is_heredoc:%d\n", file.argc - 3 - file.is_heredoc);
		printf("\nCIAO(pid:%d)\n",getpid());
		child_process(argv, file.idx++, env, &file); //sostituire pos con file.idx++
	}
	// close(file.fd[0]);
	// close(file.fd[1]);
	// close(file.filein) close(file.filein) ??
	return (EXIT_SUCCESS);
}
