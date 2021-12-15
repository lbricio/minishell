/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:47:06 by felipe            #+#    #+#             */
/*   Updated: 2021/12/14 22:11:39 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handle_cmd(int sig)
{
	(void)sig;
	printf("\n");
	g_reset_fd[2] = 130;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	reset_input();
	reset_output();
	printf("quit (code dumped)\n");
	g_reset_fd[2] = 131;
}

void	sigint_handle(int sig)
{
	if (sig == SIGINT)
	{
		write(1,"\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_reset_fd[2] = 130;
	}
}

void	handle_heredoc(int sig)
{
	(void)sig;
	g_reset_fd[2] = 130;
	exit(130);
}

void	config_sigaction(struct sigaction *act, void (*handler)(int), int sig)
{
	act->sa_handler = handler;
	act->sa_flags = 0;
	sigemptyset(&act->sa_mask);
	sigaction(sig, act, NULL);
}
