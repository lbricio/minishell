/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 21:01:01 by lbricio-          #+#    #+#             */
/*   Updated: 2022/01/04 23:24:40 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_sigquit(int sig)
{
	(void)sig;
}

/*
void	heredoc_sigquit(int sig)
{
	(void)sig;
	printf("Minihell: warning: here-document delimited");
	printf("by end-of-file(wanted `eof')\n");
	g_reset_fd[2] = 130;
	exit(130);
}*/
