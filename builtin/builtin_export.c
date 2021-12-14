/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 18:30:42 by felipe            #+#    #+#             */
/*   Updated: 2021/12/14 13:15:20 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char ***array, int size)
{
	char	**new;
	int		prev_size;
	int		i;

	prev_size = 0;
	while ((*array)[prev_size] != 0)
		prev_size++;
	new = ft_calloc(prev_size + size + 1, sizeof (char *));
	if (!new)
		return (0);
	i = -1;
	while (++i < prev_size)
		new[i] = (*array)[i];
	free(*array);
	*array = new;
	return (new);
}

t_vars	*get_value(t_vars *variables, char *var)
{
	t_vars	*iter;

	iter = variables;
	while (iter)
	{
		if (!ft_strncmp(iter->var, var, ft_strlen(var)))
			return (iter);
		iter = iter->next;
	}
	return (0);
}

int	find_env_var(char *var, char **envp)
{
	char	*equal;
	int		i;
	int		len;

	len = ft_strlen(var);
	equal = ft_strchr(var, '=');
	if (equal)
		len = equal - var;
	i = 0;
	while (envp[i])
	{
		if (!strncmp(var, envp[i], len) && \
		(envp[i][len] == '=' || envp[i][len] == 0))
			return (i);
		i++;
	}
	return (-1);
}

void	change_env(char *var, t_data *data, char ***envp)
{
	t_vars	*va;
	char	*temp;
	int		change;
	int		i;

	change = find_env_var(var, *envp);
	if (ft_strchr(var, '='))
	{
		free((*envp)[change]);
		(*envp)[change] = ft_strdup(var);
		if (!(*envp)[change])
			cleanup(data, 1);
		save_env_var(var, &i, data);
	}
	else if (get_value(data->variables, var))
	{
		free((*envp)[change]);
		va = get_value(data->variables, var);
		temp = ft_strjoin(va->var, "=");
		if (!temp)
			cleanup(data, 1);
		(*envp)[change] = ft_strjoin(temp, va->value);
		free(temp);
		if (!(*envp)[change])
			cleanup(data, 1);
	}
}

int	builtin_export(t_cmds *cmds, t_data *data, char ***envp)
{
	t_args	*iter;
	t_vars	*va;
	char	*temp;
	int		prev_size;
	int		size;
	int		i;

	size = 0;
	iter = cmds->args;
	if(!((char)iter->arg[0] >= 'a' && (char)iter->arg[0] <= 'z'
		|| (char)iter->arg[0] >= 'A' && (char)iter->arg[0] <= 'Z'))
	{
		write(1, "minishell: not a valid identifier", 33);
		write(1, "\n", 1);
		return(0);
	}
	while (iter)
	{
		if (find_env_var(iter->arg, *envp) == -1)
			size++;
		iter = iter->next;
	}
	prev_size = 0;
	while ((*envp)[prev_size] != 0)
		prev_size++;
	ft_realloc(envp, size);
	if (!(*envp))
		cleanup(data, 1);
	iter = cmds->args;
	while (iter)
	{
		if (find_env_var(iter->arg, *envp) == -1)
		{
			if (ft_strchr(iter->arg, '='))
			{
				(*envp)[prev_size] = ft_strdup(iter->arg);
				if (!(*envp)[prev_size])
					cleanup(data, 1);
				save_env_var(iter->arg, &i, data);
			}
			else if (get_value(data->variables, iter->arg))
			{
				va = get_value(data->variables, iter->arg);
				temp = ft_strjoin(va->var, "=");
				if (!temp)
					cleanup(data, 1);
				(*envp)[prev_size] = ft_strjoin(temp, va->value);
				free(temp);
				if (!(*envp)[prev_size])
					cleanup(data, 1);
			}
			else
				(*envp)[prev_size] = ft_strjoin(iter->arg, "=");
		}
		else
			change_env(iter->arg, data, envp);
		prev_size++;
		iter = iter->next;
	}
	return (0);
}
