/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:14:28 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 21:10:53 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_variable(char *line, int size, t_vars *variables)
{
	t_vars	*iter;

	iter = variables;
	while (iter)
	{
		if (!ft_strncmp(iter->var, line, size))
			return (iter->value);
		iter = iter->next;
	}
	return ("");
}

char	*find_vars(char *line, char c)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (!quote && line[i] == '$')
			return (&line[i]);
		else if (line[i] == '\'' && !quote)
			quote = 1;
		else if (line[i] == '\'' && quote)
			quote = 0;
	}
}

void	change_pipe_result(char **line, int i, t_data *data)
{
	char	*result;
	char	*temp;

	result = ft_itoa(g_reset_fd[2]);
	if (!result)
		cleanup(data, 1);
	(*line)[i] = 0;
	temp = ft_strdup(*line);
	if (!temp)
		cleanup(data, 1);
	temp = ft_concat(&temp, result);
	free(result);
	if (!temp)
		cleanup(data, 1);
	temp = ft_concat(&temp, *line + i + 2);
	free(*line);
	if (!temp)
		cleanup(data, 1);
	*line = temp;
}

void	substitute_variables(char **line, t_data *data)
{
	t_vars	*iter;
	char	*value;
	char	*vars;
	char	*temp;
	int		size;

	vars = find_vars(*line, '$');
	while (vars)
	{
		if (vars[1] == '?')
		{
			change_pipe_result(line, vars - *line, data);
			vars++;
		}
		else
		{
			vars++;
			size = 0;
			/*printf("%s\n",vars + size);*/
			while (vars[size] != ' ' && vars[size] != ';' && vars[size] != '|' && vars[size] != 0 && vars[size] != '"')
				size++;
			value = ft_strdup(get_variable(vars, size, data->variables));
			if (!value)
				cleanup(data, 1);
			vars[-1] = 0;
			temp = ft_strdup(*line);
			if (!temp)
				cleanup(data, 1);
			temp = ft_concat(&temp, value);
			free(value);
			if (!temp)
				cleanup(data, 1);
			temp = ft_concat(&temp, vars + size);
			free(*line);
			if (!temp)
				cleanup(data, 1);
			*line = temp;
		}
		vars = find_vars(*line, '$');
	}
}
