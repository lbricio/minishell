/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 22:58:18 by felipe            #+#    #+#             */
/*   Updated: 2021/12/14 13:12:48 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo_child(t_cmds *iter)
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
	exit(errno);
}

void	builtin_red(t_cmds *cmds, S_SIG **act, int builtin, char **envp)
{
	pid_t	pid;
	int		fd[2];

	pipe(fd);
	pid = fork();
	if (pid == 0)
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
		if (builtin == 1)
			ft_echo_child(cmds);
		else if (builtin == 2)
			builtin_pwd(cmds);
		else if (builtin == 3)
			builtin_env(envp, cmds);
	}
	else
	{
		close(fd[1]);
		if (cmds->fd_out == 1000)
			dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
		g_reset_fd[2] = 0;
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
