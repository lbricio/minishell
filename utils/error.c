/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 12:12:06 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/13 13:53:57 by lbricio-         ###   ########.fr       */
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
