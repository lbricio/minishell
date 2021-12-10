/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_save.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 11:27:14 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/10 01:49:49 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	save_origin_fd()
{
	g_reset_fd[0] = dup(0);
	g_reset_fd[1] = dup(1);
}

void	reset_input()
{
	dup2(g_reset_fd[0], 0);
	/*close(g_reset_fd[0]);*/
}

void	reset_output()
{
	dup2(g_reset_fd[1], 1);
	/*close(g_reset_fd[1]);*/
}
