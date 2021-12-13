/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 22:58:18 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 00:29:02 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo_child(t_cmds  *iter)
{
	t_args	*i;
	
	i = iter->args;
	while (i)
	{
		if (i->arg)
			write(iter->fd_out, i->arg, ft_strlen(i->arg));
		if (i->next)
			write(iter->fd_out, " ", 1);
		i = i->next;
	}
	if (!iter->flags)
		write(iter->fd_out, "\n", 1);
	exit(0);
}

void	ft_echo(t_cmds  *cmds, S_SIG **act)
{
	pid_t	pid;
	int		fd[2];
	int		status;
	pipe(fd);
	
	pid = fork();
	if(pid == 0)
	{
		close(fd[0]);
		if (cmds->fd_out == 0)
			reset_output();
		else if (cmds->fd_out == 1000)
		{
			dup2(fd[1], STDOUT_FILENO);
			cmds->fd_out = fd[1];
		}
		else
			dup2(cmds->fd_out, STDOUT_FILENO);
		ft_echo_child(cmds);
	}
	else
	{

		close(fd[1]);
		if (cmds->fd_out == 1000)
			dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
		close(fd[0]);
		if (cmds->fd_out != 1000 && cmds->fd_out != 0)
		{
			reset_input();
			reset_output();
		}
		if (cmds->fd_out == 0)
			reset_input();
	}
}
