/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 16:51:25 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/16 05:18:43 by rdolzi           ###   ########.fr       */
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


int main(int argc, char **argv, char **env)
{
	// fd[0] - read
	// fd[1] - write
	int		pid;
	int		pid1;
	int		fd[2];
	char	**cmd;
	int		in_file;
	int		out_file;
	
	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (argc != 5)
	{
		write(2, &"argc !=5", 8);
		return (EXIT_FAILURE  + 1);
	}
	if (pid == 0)
	{
		//Child process (cmd)
		printf("in child(cmd)...\n");
		in_file = open(argv[1], O_RDONLY);
		if(dup2(fd[1], STDIN_FILENO) == -1)
			return (EXIT_FAILURE + 2);
		close(fd[0]);
		close(fd[1]);
		cmd = get_cmd(argv[2], env);
		int i = -1;
		printf("in child(cmd)...\n");
		while (cmd[++i])
			printf("%s\n", cmd[i]);
		if(execve(cmd[0], &cmd[1], env) == -1)  // or NULL ??
		{	
			perror("execve");
			return (EXIT_FAILURE + 4);
		}
	}//
	pid1 = fork();
	if (pid1 == 0)
	{
		exit(0);
		out_file = open(argv[4], O_RDWR | O_CREAT, 0777); // O_WRONLY ??
		if (dup2(fd[0], STDIN_FILENO) == -1)
			return (EXIT_FAILURE + 3);
		close(fd[0]);
		if (dup2(out_file, STDOUT_FILENO) == -1)
			return (EXIT_FAILURE + 4);
		close(fd[1]);
		cmd = get_cmd(argv[3], env);
		if (execve(cmd[0], &cmd[1], env) == -1) // or NULL ??
		{	
			perror("execve");
			return (EXIT_FAILURE + 4);
		}
	}
	//Parent process
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, NULL, 0);
	exit(0);
	waitpid(pid1, NULL, 0);
	return (EXIT_SUCCESS);
}
