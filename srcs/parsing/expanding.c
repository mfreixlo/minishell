/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfreixo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 20:05:51 by gafreita          #+#    #+#             */
/*   Updated: 2022/08/12 20:28:22 by mfreixo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	adds the string *add to *str, in position = pos
	output is substr(str, 0, pos) + add + &str[pos]
*/
static void subs_str(char **str, char *add, int pos, int len)
{
	char *last;
	char *first;
	char	*join;

	ft_memmove(&(*str)[pos], &(*str)[pos + len + 1], ft_strlen(&(*str)[pos + len + 1]) + 1);
	if (!add)
		return;
	last = ft_substr(*str, pos, ft_strlen(&(*str)[pos]));
	first = ft_substr(*str, 0, pos);
	join = ft_strjoin(first, add);
	// free(*str);
	*str = ft_strjoin(join, last);
	free(join);
	free(last);
	free(first);
	free(add);
}

void add_spaces(char **str)
{
	int		i;
	int		j;
	char	*aux;

	i = -1;
	while ((*str)[++i])
		if (ft_isspace((*str)[i]))
			j++;
	aux = malloc(sizeof(char) * (ft_strlen((*str)) + j));
	i = 0;
	j = 0;
	while ((*str)[i])
	{
		if (ft_isspace((*str)[i]))
			aux[j++] = '\\';
		aux[j] = (*str)[i];
		j++;
		i++;
	}
	aux[j] = '\0';
	free(*str);
	*str = ft_strdup(aux);
	free(aux);
}

int expand_one(char **str, int pos, int len)
{
	char	*content;
	char	*name;
	int		new_pos;

	name = ft_substr(*str, pos, len);
	content = ft_strdup(find_env(name));
	free(name);
	if (content)
		add_spaces(&content);
	else
		ft_memmove(&(*str)[pos], &(*str)[pos + 1], ft_strlen(&(*str)[pos + 1]) + 1); //apagar o espaco a mais
	new_pos = ft_strlen(content);
	subs_str(str, content, pos -1, len);
	return (new_pos);
}

void next_exp(char **str, int *pos)
{
	int i;
	int	j;

	i = *pos;
	while ((*str)[i] && !ft_special_char((*str)[i]))
		i++;
	if ((*str)[i++] == '$')
	{
		j = i;
		while ((*str)[i] && !ft_special_char((*str)[i]) && !ft_isspace((*str)[i]))
			i++;
	}
	*pos += expand_one(str, j, i - j); // substitui o aux, pela sua versao expandida
	if (ft_isspace((*str)[i]))
		*pos += 1;
}

void expand(char **str)
{
	int i;

	i = 0;
	while ((*str)[i])
	{
		while ((*str)[i] && (*str)[i] != '$' && (*str)[i] != '\'')
			i++;
		if ((*str)[i] == '\'')
		{
			while ((*str)[++i] != '\'')
				;
			i++;
		}
		else if ((*str)[i] == '$')
		{
			next_exp(str, &i);
			// printf("next str = %s\n", *str);
			// printf("i = %d\n", i);
		}
	}
}
