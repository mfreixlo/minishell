/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfreixo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 20:05:51 by gafreita          #+#    #+#             */
/*   Updated: 2022/09/01 21:00:42 by mfreixo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	finds the quotes given by c, (" or ') in str, starting in pos = i
	pos is given as pointer so its value is altered to be used in next_arg
	returns -1 if no quotes were found (they were open but not closed)
*/
static int	check_quotes(char *str, char c, int *i)
{
	char	*aux;
	int		j;

	j = *i;
	if (!str[j])
		return (-1);
	if (str[j])
	{
		ft_memmove(&str[j], &str[j + 1], ft_strlen(&str[j + 1]) + 1);
		if (c == '\"')
			aux = ft_strchr(&str[j], c);
		else
			aux = ft_strchr(&str[j], c);
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
char	*next_arg(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		while (str[i] && !ft_isspace(str[i]) && !ft_isquote(str[i]))
			i++;
		if (!str[i] || ft_isspace(str[i]))
			return (ft_substr(str, 0, i));
		else if (ft_isquote(str[i]))
		{
			if (check_quotes(str, str[i], &i) == -1)
				return (0);
			if (!str[i])
				return (ft_substr(str, 0, i));
			continue ;
		}
		i++;
		if (!str[i])
			return (ft_substr(str, 0, i));
	}
	return (NULL);
}

/*	returns an array of args, first is the command to be executed,
	then it's its arguments
	this is the last parsing, args are ready to be executed
*/
static char	**split_command(char **str)
{
	int		i;
	char	*aux;
	int		k;
	char	**split;
	char	*aux1;

	k = 0;
	i = 0;
	aux1 = *str;
	split = malloc(sizeof(char *) * 2);
	split[0] = '\0';
	while (aux1[i])
	{
		aux = next_arg(&aux1[i]);
		if (!aux1)
		{
			free_split(split);
			split = NULL;
			break ;
		}
		i += ft_strlen(aux);
		add_split(&split, &k, aux);
		if (!aux1[i])
			break ;
		i++;
	}
	return (split);
}

/*	given the list of commands (a command ends with pipe
	of end of input from terminal),
	splits it, into valid arguments and executes
*/
void	second_parse(void)
{
	t_list	*temp;
	int		i;

	i = -1;
	temp = base()->div_pipes;
	base()->pipe.num_cmds = ft_lstsize(temp);
	base()->pipe.cmds = malloc(sizeof(char **) * (base()->pipe.num_cmds + 1));
	while (temp)
	{
		temp->content = expand((char *)temp->content);
		base()->pipe.cmds[++i] = split_command((char **)&temp->content);
		temp = temp->next;
	}
	base()->pipe.cmds[++i] = NULL;
}
