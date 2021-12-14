/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 19:41:34 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/14 18:20:26 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_pwd(t_cmds *cmds)
{
	int		fd;
	char	path[500];

	getcwd(path, 500);
	if (cmds->fd_out == 0)
		fd = 1;
	else if (cmds->fd_out == 1000)
		fd = 0;
	else
		fd = cmds->fd_out;
	write(fd, path, ft_strlen(path));
	write(fd, "\n", 1);
	exit(0);
}
