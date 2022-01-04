/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:23 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/04 19:08:17 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_sintax(char *line, t_cmds *iter, t_data *data, t_sig **act)
{
	if (sintax_check(line) == -1)
		return (0);
	iter->fd_in = 0;
	if (ft_strchr(line, '<'))
	{
		line = treat_input_red(line, iter, act);
		if (!line)
			cleanup(data, 2);
	}
	return (1);
}

int	create_new_cmds(t_cmds **iter, t_data *data)
{
	(*iter)->next = malloc(sizeof (t_cmds));
	if (!(*iter)->next)
		cleanup(data, 2);
	(*iter) = (*iter)->next;
	init_cmds(*iter);
	return (1);
}

void	parser_utils(t_data *data, char *line, t_cmds *iter, int *j)
{
	while (line[*j] == ' ')
		(*j)++;
	save_env_var(line + *j, j, data, 0);
	iter->cmd = get_cmd(line + *j, j, data);
	remove_char(iter->cmd, get_quote(iter->cmd));
	iter->flags = get_flags(line + *j, j, data);
	iter->args = get_args(line + *j, j, data);
	get_redirect(line + *j, j, iter);
	while (line[*j] == ' ')
		(*j)++;
}

void	clear_line(char *line)
{
	int	j;

	j = 0;
	while (line[j] != '\0')
		line[j++] = ' ';
}

int	parser(char *line, t_data *data, char ***envp, t_sig **act)
{
	t_cmds	*iter;
	int		j;

	data->cmds = malloc(sizeof (t_cmds));
	if (!data->cmds)
		cleanup(data, 2);
	init_cmds(data->cmds);
	iter = data->cmds;
	j = 0;
	while (line[j] != 0 && line[j] != ';')
	{
		while (line[j] == ' ')
			j++;
		if (get_quote(line) == 0)
			if (!check_sintax(line + j, iter, data, act))
				clear_line(line);
		parser_utils(data, line, iter, &j);
		if (iter->cmd[0] != '\0')
			if (check_cmds(iter, *envp))
				return (-1);
		if (line[j] == '|' && line[j + 1] != '|')
			j += create_new_cmds(&iter, data);
	}
	return (0);
}
