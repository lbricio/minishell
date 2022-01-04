/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 12:12:06 by lbricio-          #+#    #+#             */
/*   Updated: 2022/01/04 14:10:21 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_file(char *file)
{
	write(2, "minishell: ", 11);
	write(2, file, ft_strlen(file));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	return (127);
}

int	sintax_error(void)
{
	write(1, "sintax error near unexpected token", 35);
	write(1, "\n", 1);
	g_reset_fd[2] = 2;
	return (-1);
}

int	flag_error(t_cmds *cmds)
{
	t_cmds	*iter;
	t_cmds	*next;
	t_args	*next_arg;

	printf("%s: invalid option -- \'%s\'\n", cmds->cmd, &cmds->flags[1]);
	iter = cmds;
	while (iter != 0)
	{
		if (iter->flags)
			free(iter->flags);
		while (iter->args)
		{
			next_arg = iter->args->next;
			free(iter->args);
			iter->args = next_arg;
		}
		next = iter->next;
		free(iter);
		iter = next;
	}
	return (2);
}

int	cmd_error(t_cmds *cmds)
{
	printf("%s: command not found\n", cmds->cmd);
	g_reset_fd[2] = 127;
	return (1);
}

int	file_error(t_cmds *cmds)
{
	printf("%s: No such file or directory\n", cmds->cmd);
	g_reset_fd[2] = 127;
	return (1);
}
