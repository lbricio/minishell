/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 22:22:04 by felipe            #+#    #+#             */
/*   Updated: 2021/12/12 17:52:09 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_cd(t_cmds *cmds)
{
	int	result;

	if (cmds->args->next)
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	result = chdir(cmds->args->arg);
	if (result == -1)
	{
		printf("cd: %s: No such file or directory\n", cmds->args->arg);
		return (1);
	}
	return (0);
}
