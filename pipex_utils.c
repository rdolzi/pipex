/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 00:56:36 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/16 03:02:55 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// restituisce la stringa completa del comando
// if not PATH= ?
char	**get_cmd(char *argv, char **env)
{
	int		i;
	//setta errore se comando non valido ?
	char	**cmd_opt;
	// char	**base;

	i = 0;
	cmd_opt = ft_split(argv, ' ');
	// base = ft_split(env);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			printf("NORMAL>%s\n",env[i]);
			printf("MODIFIED>%s",env[i]+5);
		}
		i++;
	}
	// printf("%s",env[0]);
	if (!cmd_opt)
		exit (EXIT_FAILURE + 5);
	//entra in env e cerca la stringa contenente PATH=  
	//splitta la stringa (:), inserisce '/' e verifica se il comando esiste
	// se trova il  comando ritorna il comando cmd, altrimenti ritorna errore

	return (cmd_opt);
}