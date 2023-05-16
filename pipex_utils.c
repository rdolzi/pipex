/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdolzi <rdolzi@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 00:56:36 by rdolzi            #+#    #+#             */
/*   Updated: 2023/05/16 05:27:10 by rdolzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
char	**get_cmd(char *argv, char **env)
{
	int		i;
	//setta errore se comando non valido ?
	char	*cmd;
	char	**cmd_opt;
	char	**base;

	i = -1;
	base = NULL;
	cmd_opt = ft_split(argv, ' ');
	//entra in env e cerca la stringa contenente PATH=  
	//splitta la stringa (:),
	while (env[++i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			// printf("NORMAL>%s\n",env[i]);
			// printf("MODIFIED>%s\n",env[i]+5);
			base = ft_split((env[i]+ 5), ':');
			break ;
		}
	}
	i = -1;
	// inserisce '/' e verifica se il comando esiste
	// se trova il  comando ritorna il comando cmd, altrimenti ritorna errore
	while (base[++i])
	{

		cmd = ft_strjoin(ft_strjoin(base[i], "/"),cmd_opt[0]);
		// printf(">%s\n", cmd);
		if (access(cmd, X_OK) == 0)
		{
			// printf(">>>>>>>");
			free(cmd_opt[0]);
			cmd_opt[0] = cmd;
			free_all(base, NULL);
			return (cmd_opt);
		}
	}
	free_all(base, cmd_opt);
	exit (EXIT_FAILURE + 6); //chiude tutti gli fd?
}
