/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:47:06 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 20:54:46 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Do a break line
// Save the status 130
void	sigint_handle_cmd(int sig)
{
	(void)sig;
	printf("\n");
	g_reset_fd[2] = 130;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	printf("quit (code dumped)\n");
	g_reset_fd[2] = 131;
}

// Print a new command line
// Save the status 130
void	sigint_handle(int sig)
{
	if (sig == SIGINT)
	{
		write(1,"\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_reset_fd[2] = 130;
	}
}

// Do a break line
// Exit the process with status 130
void	handle_heredoc(int sig)
{
	(void)sig;
	write(1,"\n", 1);
	exit(130);
}

// Set up a sigaction handle signal
void	config_sigaction(struct sigaction *act, void (*handler)(int), int sig)
{
	act->sa_handler = handler;
	act->sa_flags = 0;
	sigemptyset(&act->sa_mask);
	sigaction(sig, act, NULL);
}
