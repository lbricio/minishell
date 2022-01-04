/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:49:39 by lbricio-          #+#    #+#             */
/*   Updated: 2022/01/04 20:21:06 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strword(const char *s, char *dup)
{
	int		i;
	int		size;

	size = 0;
	while ((s[size] >= 'a' && s[size] <= 'z')
		|| (s[size] >= 'A' && s[size] <= 'Z')
		|| (s[size] >= '0' && s[size] <= '9')
		|| s[size] == '.')
		size++;
	dup = malloc(size + 1);
	if (!dup)
		return (0);
	i = 0;
	while (i < size)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	char	*alloced;

	if (nmemb == 0 || size == 0)
	{
		alloced = malloc(1);
		*alloced = 0;
		return (alloced);
	}
	alloced = malloc(nmemb * size);
	if (!alloced)
		return (0);
	i = 0;
	while (i < nmemb * size)
	{
		alloced[i] = 0;
		i++;
	}
	return (alloced);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] != 0 && s2[i] != 0 && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i] || i == n - 1)
			break ;
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	int		len1;
	int		len2;
	int		i;

	len1 = 0;
	while (s1[len1] != 0)
		len1++;
	len2 = 0;
	while (s2[len2] != 0)
		len2++;
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (0);
	i = 0;
	while (i < len1 || i < len2)
	{
		if (i < len1)
			joined[i] = s1[i];
		if (i < len2)
			joined[i + len1] = s2[i];
		i++;
	}
	joined[len2 + len1] = 0;
	return (joined);
}

int	is_builtin(char *cmd)
{
	int	len;

	if (cmd)
	{
		len = ft_strlen(cmd);
		if (!ft_strncmp(cmd, "echo", len))
			return (1);
		else if (!ft_strncmp(cmd, "cd", len))
			return (1);
		else if (!ft_strncmp(cmd, "pwd", len))
			return (1);
		else if (!ft_strncmp(cmd, "export", len))
			return (1);
		else if (!ft_strncmp(cmd, "unset", len))
			return (1);
		else if (!ft_strncmp(cmd, "env", len))
			return (1);
		else if (!ft_strncmp(cmd, "exit", len))
			return (1);
		return (0);
	}
	return (1);
}
