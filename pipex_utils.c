/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 00:56:36 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/18 04:08:08 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = -1;
	while (matrix[++i])
		free(matrix[i]);	
	free(matrix);
}

// funziona bloccando il free dello strjoin
static void	free_all(char **base, char **cmd_opt)
{
	int	i;

	i = -1;
	if (base)
	{
		while (base[++i])
		{
			//printf("@%s | addr: %p\n", base[i], base[i]);
			free(base[i]);	
		}
		free(base);
	}
	i = -1;
	if (cmd_opt)
	{
		while (cmd_opt[++i])
			free(cmd_opt[i]);
		free(cmd_opt);
	}

}


// restituisce la stringa completa del comando
// if not PATH= ?
char	**get_cmds(char *argv, char **env)
{
	int		i;
	//setta errore se comando non valido ?
	char	*cmd;
	char	*partial_cmd;
	char	**cmd_opt;
	char	**base;

	i = 0;
	base = NULL;
	cmd_opt = ft_split(argv, ' ');
	//entra in env e cerca la stringa contenente PATH=  
	//splitta la stringa (:),
	while (ft_strncmp(env[i], "PATH=", 5))
		i++;
	//printf(">>%s\n", env[i]);
	base = ft_split((env[i]+ 5), ':');
	// i = -1;
	// while (base[++i])
	// 	printf(">>%s\n", base[i]);
	// exit(0);
	i = -1;
	// inserisce '/' e verifica se il comando esiste
	// se trova il  comando ritorna il comando cmd, altrimenti ritorna errore
	while (base[++i])
	{
		partial_cmd = ft_strjoin(base[i], "/");
		cmd = ft_strjoin(partial_cmd, cmd_opt[0]);
		free(partial_cmd);
		// printf(">%s\n", cmd);
		if (access(cmd, X_OK) == 0)
		{
			free(cmd_opt[0]);
			cmd_opt[0] = cmd;
			printf("0>%s", cmd_opt[0]);
			printf("1>%s", cmd_opt[1]);
			exit(0);
			free_all(base, NULL);
			return (cmd_opt);
		}
	}
	free_all(base, cmd_opt);
	exit (EXIT_FAILURE + 6); //chiude tutti gli fd?
}
