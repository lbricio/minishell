/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 18:30:42 by felipe            #+#    #+#             */
/*   Updated: 2021/12/20 11:59:07 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char ***array, int size, t_data *data)
{
	char	**new;
	int		prev_size;
	int		i;

	prev_size = 0;
	while ((*array)[prev_size] != 0)
		prev_size++;
	new = ft_calloc(prev_size + size + 1, sizeof (char *));
	if (!new)
		cleanup(data, 2);
	i = -1;
	while (++i < prev_size)
	{
		new[i] = (*array)[i];
	}
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

void	export_tvars(char *var, int change, char ***envp, t_data *data)
{
	t_vars	*va;
	char	*temp;

	free((*envp)[change]);
	va = get_value(data->variables, var);
	temp = ft_strjoin(va->var, "=");
	if (!temp)
		cleanup(data, 2);
	(*envp)[change] = ft_strjoin(temp, va->value);
	if (!(*envp)[change])
	{
		while (--change >= 0)
			free((*envp)[change]);
		cleanup(data, 2);
	}
	free(temp);
}

void	change_env(char *var, t_vars **variables, char ***envp, t_data *data)
{
	int		change;
	int		i;

	change = find_env_var(var, *envp);
	if (ft_strchr(var, '='))
	{
		free((*envp)[change]);
		(*envp)[change] = ft_strdup(var);
		if (!(*envp)[change])
		{
			while (--change >= 0)
				free((*envp)[change]);
			cleanup(data, 2);
		}
		save_env_var(var, &i, data, 1);
	}
	else if (get_value(*variables, var))
		export_tvars(var, change, envp, data);
}

void	init_sizes(int *size, int *prev_size, t_cmds *cmds, char ***envp)
{
	t_args	*iter;

	(*size) = 0;
	iter = cmds->args;
	while (iter)
	{
		if (find_env_var(iter->arg, *envp) == -1)
			(*size)++;
		iter = iter->next;
	}
	*prev_size = 0;
	while ((*envp)[*prev_size] != 0)
		(*prev_size)++;
}

void	new_env_var(char ***envp, int prev_size, t_data *data, t_args *iter)
{
	t_vars	*va;
	char	*temp;

	if (ft_strchr(iter->arg, '='))
	{
		(*envp)[prev_size] = ft_strdup(iter->arg);
		if (!(*envp)[prev_size])
			cleanup(data, 2);
		save_env_var(iter->arg, NULL, data, 1);
	}
	else if (get_value(data->variables, iter->arg))
	{
		va = get_value(data->variables, iter->arg);
		temp = ft_strjoin(va->var, "=");
		(*envp)[prev_size] = ft_strjoin(temp, va->value);
		free(temp);
		if (!(*envp)[prev_size])
			cleanup(data, 2);
	}
	else
	{
		(*envp)[prev_size] = ft_strjoin(iter->arg, "=");
		if (!(*envp)[prev_size])
			cleanup(data, 2);
	}
}

int	builtin_export(t_cmds *cmds, t_vars **variables, char ***envp, t_data *data)
{
	t_args	*iter;
	int		prev_size;
	int		size;

	iter = cmds->args;
	if (!(((char)iter->arg[0] >= 'a' && (char)iter->arg[0] <= 'z')
			|| ((char)iter->arg[0] >= 'A' && (char)iter->arg[0] <= 'Z')))
	{
		write(1, "minishell: not a valid identifier\n", 34);
		g_reset_fd[2] = 1;
		return (0);
	}
	init_sizes(&size, &prev_size, cmds, envp);
	ft_realloc(envp, size, data);
	while (iter)
	{
		if (find_env_var(iter->arg, *envp) == -1)
			new_env_var(envp, prev_size, data, iter);
		else
			change_env(iter->arg, variables, envp, data);
		prev_size++;
		iter = iter->next;
	}
	return (0);
}
