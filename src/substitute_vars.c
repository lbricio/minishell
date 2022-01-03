/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:20:04 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:20:05 by lufelipe         ###   ########.fr       */
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

char	*find_vars(char *line)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (!quote && line[i] == '$' && (line[i + 1] != ' ' && \
		line[i + 1] != 0 && line[i + 1] != '|' && line[i + 1] != '<' \
		&& line[i + 1] != '>'))
			return (&line[i]);
		else if (line[i] == '\'' && !quote)
			quote = 1;
		else if (line[i] == '\'' && quote)
			quote = 0;
	}
	return (0);
}

void	change_pipe_result(char **line, int i)
{
	char	*result;
	char	*temp;

	result = ft_itoa(g_reset_fd[2]);
	(*line)[i] = 0;
	temp = ft_strdup(*line);
	temp = ft_concat(&temp, result);
	free(result);
	temp = ft_concat(&temp, *line + i + 2);
	free(*line);
	*line = temp;
}

void	substitute_utils(char **vars, char **line, t_data *data)
{
	char	*temp;
	char	*value;
	int		size;

	(*vars)++;
	size = 0;
	while ((*vars)[size] != ' ' && (*vars)[size] != ';' && (*vars)[size] != '|'
		&& (*vars)[size] != 0 && (*vars)[size] != '"')
		size++;
	value = ft_strdup(get_variable((*vars), size, data->variables));
	if (!value)
		cleanup(data, 2);
	(*vars)[-1] = 0;
	temp = ft_strdup(*line);
	if (!temp)
		cleanup(data, 2);
	temp = ft_concat(&temp, value);
	free(value);
	if (!temp)
		cleanup(data, 2);
	temp = ft_concat(&temp, (*vars) + size);
	free(*line);
	if (!temp)
		cleanup(data, 2);
	*line = temp;
}

void	substitute_variables(char **line, t_data *data)
{
	char	*vars;

	vars = find_vars(*line);
	while (vars)
	{
		if (vars[1] == '?')
		{
			change_pipe_result(line, vars - *line);
			vars++;
		}
		else
			substitute_utils(&vars, line, data);
		vars = find_vars(*line);
	}
}
