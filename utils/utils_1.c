/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:48:45 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/14 16:45:39 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strnstr(const char	*big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && i + j < len)
		{
			if (little[j + 1] == '\0')
				return ((char *)big + i);
			j++;
		}
		i++;
	}
	return (0);
}

char	*cmds_to_string(t_cmds *cmds) //FORMATO DATA
{
	t_args	*iter;
	char	*line;
	char	*temp;

	line = ft_strdup(cmds->cmd);
	if (cmds->flags)
	{
		temp = ft_strjoin(line, " ");
		free(line);
		line = ft_strjoin(temp, cmds->flags);
		free(temp);
	}
	iter = cmds->args;
	while (iter && iter->arg != 0)
	{
		temp = ft_strjoin(line, " ");
		free(line);
		line = ft_strjoin(temp, iter->arg);
		free(temp);
	}
	return (line);
}

char	*ft_concat(char **dest, char *src)
{
	char	*new;
	int		i;
	int		len_dest;
	int		len_src;

	len_dest = ft_strlen((*dest));
	len_src = ft_strlen(src);
	new = malloc(len_dest + len_src + 1);
	if (!new)
	{
		free((*dest));
		return (0);
	}
	i = -1;
	while (++i < len_dest)
		new[i] = (*dest)[i];
	while (i < len_dest + len_src)
	{
		new[i] = src[i - len_dest];
		i++;
	}
	new[i] = 0;
	free((*dest));
	return (new);
}
