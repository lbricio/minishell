/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 21:17:18 by felipe            #+#    #+#             */
/*   Updated: 2021/12/18 18:23:49 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;

	i = 0;
	while (s[i] != 0)
		i++;
	dup = malloc(i + 1);
	if (!dup)
		return (0);
	i = 0;
	while (s[i] != 0)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = s[i];
	return (dup);
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len] != 0)
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	if (c >= 256)
		c = c - 256;
	i = 0;
	while (s[i] != 0)
	{
		if (s[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == c)
		return ((char *)&s[i]);
	return (0);
}

void	lstadd_back(t_data *data, t_vars *new) //FORMATO DATA
{
	t_vars	*last;

	last = data->variables;
	if (last == 0)
	{
		data->variables = new;
		return ;
	}
	while (last->next != 0)
		last = last->next;
	last->next = new;
}

/* funcao q duplica uma str s ate o tamanho len */
char	*ft_strndup(const char *s, int len)
{
	char	*dup;
	int		i;

	i = 0;
	while (s[i] != 0 && i < len)
		i++;
	dup = malloc(i + 1);
	if (!dup)
		return (0);
	i = 0;
	while (s[i] != 0 && i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

