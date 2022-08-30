/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gafreita <gafreita@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 19:50:37 by gafreita          #+#    #+#             */
/*   Updated: 2022/08/30 17:26:01 by gafreita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	Acts as global variable, returns the up-to-date base*/
t_base	*base(void)
{
	static t_base	base;

	return (&base);
}

/*	Initializes all varibles from base()*/
void	ini_base(char **env)
{
	char	*paths;
	char	*shlvl;

	ini_env(env);
	if (find_env("SHLVL"))
		shlvl = ft_itoa(ft_atoi(find_env("SHLVL")) + 1);
	else
		shlvl = ft_itoa(1);
	change_var("SHLVL", shlvl);
	free(shlvl);
	paths = getenv("PATH");
	(base()->paths) = ft_split(paths, ':');
	(base()->home) = ft_strdup(getenv("HOME"));
	(base()->errnumb) = 0;
	(base()->oldpwd) = ft_strdup(find_env("OLDPWD"));
}

/*frees the command line after execution*/
void	free_command_line(void)
{
	int	i;

	i = -1;
	while (base()->pipe.cmds[++i])
		free_split(base()->pipe.cmds[i]);
	free_split(base()->pipe.cmds[i]);
	free(base()->pipe.cmds);
	ft_lstclear(&(base()->div_pipes), free);
}

/*frees everything from struct base, before quitting the program*/
void	base_free(void)
{
	env_free();
	free(base()->home);
	free_split(base()->paths);
	ft_lstclear(&base()->div_pipes, free);
}
