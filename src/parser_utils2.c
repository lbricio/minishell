/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:17 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:19:18 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// translate -l -h -a into -lha
char	*trunc_flags(char *flags, t_data *data)
{
	char	*new_str;
	char	*newest_str;
	int		i;
	int		size;

	new_str = ft_calloc(ft_strlen(flags) + 1, sizeof(char));
	if (!new_str)
		cleanup(data, 2);
	if (flags[0] != '-')
		return (0);
	i = 0;
	size = 0;
	new_str[0] = '-';
	while (flags[++i] != 0)
	{
		if ((flags[i] >= 'a' && flags[i] <= 'z') || \
		(flags[i] >= 'A' && flags[i] <= 'Z'))
			new_str[++size] = flags[i];
	}		
	new_str[++size] = '\0';
	newest_str = ft_strdup(new_str);
	free(new_str);
	if (!newest_str)
		cleanup(data, 2);
	return (newest_str);
}

int	get_flag_utils(char *line)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (line[i] != 0 && line[i] != ' ' && \
	line[i] != '|' && line[i] != ';')
	{
		if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
		if (line[i] == ' ')
			if (line[i + 1] == '-')
				i += 2;
	}
	return (i);
}

/* retorna as flags como uma string */
char	*get_flags(char *line, int *count, t_data *data)
{
	char	*flags;
	char	*trunc;
	char	quote;
	int		i;
	int		j;

	j = -1;
	while (line[++j] == ' ')
		(*count)++;
	if (line[j] == '-' || ((line[j] == '"' || \
	line[j] == '\'') && line[j + 1] == '-'))
	{
		i = get_flag_utils(line + j);
		flags = ft_strndup(line + j, i);
		if (!flags)
			cleanup(data, 2);
		quote = get_quote(flags);
		remove_char(flags, quote);
		(*count) += i;
		trunc = trunc_flags(flags, data);
		free(flags);
		return (trunc);
	}
	return (0);
}
