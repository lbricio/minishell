/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 20:13:44 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 21:59:17 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_error(int err, char *arg)
{
	if (err == 1)
		printf("exit: too many arguments\n");
	else if (err == 2)
		printf("exit: %s: numeric argument required\n", arg);
	return (err);
}

static int	check_arg(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i] != 0)
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (2);
	}
	return (0);
}

int	builtin_exit(t_cmds *cmds, t_data *data)
{
	unsigned char	err;
	t_args			*iter;
	int				size;

	size = 0;
	iter = cmds->args;
	while (iter && iter->arg)
	{
		size++;
		iter = iter->next;
	}
	if (cmds->args->arg && check_arg(cmds->args->arg))
		err = exit_error(2, cmds->args->arg);
	else if (size > 1)
		return (exit_error(1, 0));
	else if (size == 1)
		err = (unsigned char)ft_atoi(cmds->args->arg);
	else
		err = 0;
	cleanup(data, 1);
	exit(err);
}
