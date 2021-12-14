/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 12:12:06 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/13 21:41:42 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_file(char *file)
{
	write(2, "minishell: ", 11);
	write(2, file, ft_strlen(file));
	write(2, " :", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	/*if (2 == 3)
		g_reset_fd[2] = 1;*/
	return (127);
}

int sintax_error(void)
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
	exit(0);
}

void	cleanup(t_data *data, int end)
{
	t_cmds	*i_cmds;
	t_args	*i_args;
	void	*next;

	i_cmds = data->cmds;
	while (i_cmds)
	{
		i_args = i_cmds->args;
		while (i_args)
		{
			next = i_args->next;
			if(i_args->arg)
				free(i_args->arg);
			free(i_args);
			i_args = next;
		}
		next = i_cmds->next;
		free(i_cmds->cmd);
		free(i_cmds->flags);
		i_cmds = next;
	}
	if (end)
		cleanup_variables(data);
}
