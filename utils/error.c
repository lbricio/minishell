/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 12:12:06 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/14 13:05:28 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_file(char *file)
{
	write(2, "minishell: ", 11);
	write(2, file, ft_strlen(file));
	write(2, " :", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	return (127);
}

int	sintax_error(void)
{
	write(1, "sintax error", 13);
	write(1, "\n", 1);
	g_reset_fd[2] = 2;
	return (-1);
}

int	error_handler(int n)
{
	if (n == 1)
		printf("Not enough space\n");
	exit(n);
}

void	cleanup_variables(t_data *data)
{
	t_vars	*iter;
	t_vars	*next;
	int		i;

	iter = data->variables;
	while (iter)
	{
		free(iter->var);
		free(iter->value);
		next = iter->next;
		free(iter);
		iter = next;
	}
	i = -1;
	while ((*data->envp)[++i] != 0)
		free((*data->envp)[i]);
	free(*data->envp);
}

void	free_args(t_args **args)
{
	t_args	*iter;
	t_args	*next;

	iter = *args;
	while (iter)
	{
		if (iter->arg)
			free(iter->arg);
		next = iter->next;
		free(iter);
		iter = next;
	}
}

void	free_cmds(t_cmds **cmds)
{
	t_cmds	*iter;
	t_cmds	*next;

	iter = *cmds;
	while (iter)
	{
		if (iter->args)
			free_args(&iter->args);
		if (iter->flags)
			free(iter->flags);
		if (iter->cmd)
			free(iter->cmd);
		next = iter->next;
		free(iter);
		iter = next;
	}
}

void	cleanup(t_data *data, int end)
{
	free_cmds(&data->cmds);
	if (end)
		cleanup_variables(data);
}
