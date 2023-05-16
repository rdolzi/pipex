/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   theory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 09:55:10 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/15 23:15:42 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// limiter https://stackoverflow.com/questions/2500436/how-does-cat-eof-work-in-bash
// ps aux | grep 49552
// --PIPEX--
// Func:
// > open, close, read, write
// > malloc, free, exit
// > perror, strerror [V]
// > access [V]
// ? dup, dup2
// ? fork, pipe, waitpid
// ? unlink, excve

// // ----------PERROR / STRERROR-----------------
// https:// www.youtube.com/watch?v=IZiUT-ipnj0

// 1> PERROR

// SYNOPSIS:
// #include <stdio.h>
// void perror(const char *s);

// DEF:
// The perror() function finds the error message corresponding to the current
//      value of the global variable errno (intro(2)) and writes it, followed by a
//      newline, to the standard error file descriptor.  If the argument s is
//      non-NULL and does not point to the null character, this string is prepended
//      to the message string and separated from it by a colon and space (“: ”);
// otherwise, only the error message string is printed.If the error number is not recognized, these functions return an error message string containing “Unknown error : ” followed by the error number in decimal.

// 1> STRERROR

// SYNOPSIS:
// #include <string.h>
// char * strerror(int errnum);

// DEF:
// The strerror() function accepts an error number argument errnum and returns
//     a pointer to the corresponding message string.

// ------------ACCESS-------------------
// https://www.qnx.com/developers/docs/6.4.1/neutrino/lib_ref/a/access.html

//DEF : Check to see if a file or directory can be accessed

// SYNOPSIS:
// #include <unistd.h>
// int access( const char * path, int amode );

//ARGUMENTS:
// path
//     The path to the file or directory that you want to access.

// amode:
//     The access mode you want to check. This must be either:
//     >F_OK — test for file existence.
//      
    // or a bitwise ORing of the following access permissions to be checked, as defined in the header <unistd.h>:
//     >R_OK — test for read permission.
//     >W_OK — test for write permission.
//     >X_OK — for a directory, test for search permission. 
//             Otherwise, test for execute permission.

// RETURN : 
//     >0 The file or directory exists and can be accessed with the specified mode.
//     >1 An error occurred(errno is set).

//read && access
#include <unistd.h>
//oflags
#include <fcntl.h>
//perror
#include <stdio.h>
//strerror
#include <string.h>
//errno
#include <errno.h>

#include <sys/wait.h>

// EXIT_SUCCESS - EXIT_ERROR
#include <stdlib.h>
extern int errno;

// int main ()
// {
//     char *file = "test.txt";
//     // int i = open("nonexist.txt", O_RDONLY);
//     // if (i == -1)
//     //     perror("test error");
//     // printf("errno: %d\n", errno);
//     // printf("test error: %s", strerror(errno));

//     // int i = access(file, F_OK | R_OK | W_OK | X_OK);
//     // if (i == -1)
//     //     perror("access error");

//     int id = fork();

//     int i;
//     if (id != 0)
//     {
//         i = 5;
//         // printf("in the parent...\n");
//         // printf("getpid:%d| id:%d |getppid:%d|\n", getpid(), id, getppid());
//     }
//     else 
//     {
//         printf("child id:%d\n", getpid());
//         i = 0;
//         // printf("in the child...\n");
//         // printf("getpid:%d| id:%d |getppid:%d|\n", getpid(), id, getppid());
//     }
//     if (id != 0)
//         printf("dead_child:%d\n",wait(NULL));
//     int n = -1;
//     while (++n < 5)
//     {
//         printf("%d", ++i);
//         //fflush(stdout);
//     }
//     printf("\n");
//     if (id == 0)
//     {
//         if (wait(NULL) == -1)
//              printf("no child to wait for...\n");
//     }
//     return (0);
// }

// int main()
// {
//     // fd[0] - read
//     // fd[1] - write
//     int fd[2];
//     int pid;
//     int x;

//     if (pipe(fd) == -1)
//     {
//         perror("pipe");
//         return (EXIT_FAILURE);
//     }
//     pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         return (3);
//     }
//     if (pid == 0)
//     {

//         close(fd[0]);
//         printf("ENTER A NUMBER: ");
//         scanf("%d", &x);
//         if (write(fd[1], &x, sizeof(int)) == -1)
//         {
//             printf("Error while writing to the pipe\n");
//             return (1);
//         }
//         close(fd[1]);
//     }
//     else
//     {

//         close(fd[1]);
//         if (read(fd[0],&x, sizeof(int)) == -1)
//         {
//             printf("Error while reading from the pipe \n");
//             return (2);
//         }
//         close(fd[0]);
//         printf("Number received in parent: %d\n", x);
//     }
//     return EXIT_SUCCESS;
// }

// int main(void)
// {
//     int i = -1;
//     char *argv[] = {"ciao", "come", "stai", NULL};
//     char *env[] = {"env1", "env2", "env3", NULL};
//     execve("./exe", argv, env);
// }

int main(int argc, char **argv, char **env)
{
    // fd[0] - read
    // fd[1] - write
    
    int i = -1;
    int fd[2];
    if (pipe(fd) == -1)
        return (2);
    int pid = fork();
    if (pid == -1)
        return (1);
  

    // int file = open("file.txt", O_RDWR| O_CREAT, 0777);
    // char *env[] = {"env1", "env2", "env3", NULL};
    //execve("./exe", argv, env);
    if (pid == 0)
    {
        char *arg[] = {"/sbin/ping", "-c", "5", "google.com" , NULL};
        close(fd[0]);
       
        int f2 = dup2(fd[1] ,STDOUT_FILENO);
        close(fd[1]);
        // printf("f2:%d\n", f2);
        if(execve("/sbin/ping", arg, env) == -1)
            return (3);
    }


   
    int pid2 = fork();

    if (pid2 == 0)
    {
        // Child process (grep)
        char *arg2[] = {"/usr/bin/grep", "round-trip", NULL};
        int fd2 = dup2(fd[0], STDIN_FILENO);
        // close(fd[0]);
        close(fd[1]);
        if (execve("/usr/bin/grep", arg2, env) == -1)
            return (4);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid, NULL, 0);
    // waitpid(pid2, NULL, 0);
    // waitpid(pid2, NULL, 0);
    // char *str;
    // int i = read(fd[0],str, 1000);
    // str[i] = 0;
    // printf("%s", str);
    // close(fd[0]);
    // close(fd[1]);
        
}

