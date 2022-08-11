/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfreixo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 20:05:51 by gafreita          #+#    #+#             */
/*   Updated: 2022/08/11 09:14:41 by mfreixo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	finds the quotes given by c, (" or ') in str, starting in pos = i
	pos is given as pointer so its value is altered to be used in next_arg
	returns -1 if no quotes were found (they were open but not closed)	
*/
static int    check_quotes(char *str, char c, int *i)
{
	char *aux;
	int j;

	j = *i;
	if (!str[j])
		return (-1);
	if (j > 0)
	{
		if (str[j - 1] && str[j - 1] == '\\')
		{
			ft_memmove(&str[j - 1], &str[j], ft_strlen(&str[j]) + 1);
			*i = j;
			return (0);
		}
	}
	if (str[j])
	{
		ft_memmove(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
		aux = ft_strchr_valid(&str[j], c);
		if (!aux)
			return (-1);
		*i += ft_strlen(&str[j]) - ft_strlen(aux);
		ft_memmove(aux, aux + 1, ft_strlen(aux + 1) + 1);
	}
	else
		return (-1);
	return (0);
}

/*returns the next arg, it can end in valid space, be delimited by ", '*/
static char    *next_arg(char *str)
{
	int i;
	// char *aux;
	// char c;

	i = 0;
	while (str[i])
	{
		while (str[i] && !ft_isspace(str[i]) && !ft_isquote(str[i]) && str[i] != '\\' && str[i] != '$')
			i++;
		if (!str[i] || ft_isspace(str[i]))
			return (ft_substr(str, 0, i));
		else if (str[i] == '\\' && str[i + 1] && ft_isspace(str[i + 1]))
			ft_memmove(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
		else if (ft_isquote(str[i]))
		{
			if (check_quotes(str, str[i], &i) == -1)
				return (0);
			if (!str[i])
				return (ft_substr(str, 0, i));
			continue;
		}
		i++;
	}
	return (NULL);
}

/*	returns an array of args, first is the command to be executed,
	then it's its arguments
	this is the last parsing, args are ready to be executed
*/
static char **split_command(char *str)
{
	int i;
	char    *aux;
	int     k;
	char    **split;

	k = 0;
	i = 0;
	expand(&str); //este expand nao esta a funcionar
	split = malloc(sizeof(char*) * 2);
	split[0] = '\0';
	while (str[i])
	{
		aux = next_arg(&str[i]);
		if (!aux)
		{
			free_split(split);
			split = NULL;
			break ;
		}
		i += ft_strlen(aux);
		add_split(&split, &k, aux);
		if (!str[i])
			break;
		i++;
	}
	return (split);
}

/*	given the list of commands (a command ends with pipe of end of input from terminal),
	splits it, into valid arguments and executes */
void	second_parse(void)
{
	t_list	*temp;
	char	**split;
	// int		i;

	// i = 0;
	temp = base()->cmds;
	while (temp)
	{
		// printf("line = %s\n", (char *) temp->content);
		split = split_command((char *)temp->content);
		// execute(split);
		// i = 0;
		// while (split[i])
		// {
		// 	printf(">>%s<<\n", split[i]);
		// 	i++;
		// }
		// printf("----------\n");
		temp = temp->next;
		free_split(split);
	}
	ft_lstclear(&(base()->cmds), free); //esta a dar double free aqui, nao sei bem pq
}
