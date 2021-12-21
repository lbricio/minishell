/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:19:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/21 12:56:21 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_builtin_name(char *cmd, char *name)
{

	if(ft_strlen(cmd) == ft_strlen(name))
	{
		if (!(strncmp(cmd, name, ft_strlen(cmd))))
			return (1);
	}
	return (0);
}

void	exec_builtin(t_cmds *cmds, t_data *data, char ***envp, S_SIG **act)
{
	t_cmds	*iter;

	iter = cmds;
	while (iter != 0)
	{
		if (ft_strlen(iter->cmd) && check_builtin_name(iter->cmd, "env"))
			builtin_red(cmds, act, 3, *envp);
		else if (ft_strlen(iter->cmd) && check_builtin_name(iter->cmd, "echo"))
			builtin_red(cmds, act, 1, *envp);
		else if (ft_strlen(iter->cmd) && check_builtin_name(iter->cmd, "pwd"))
			builtin_red(cmds, act, 2, *envp);
		else if (ft_strlen(iter->cmd) && check_builtin_name(iter->cmd, "exit"))
			builtin_exit(iter, data);
		else if (ft_strlen(iter->cmd) && check_builtin_name(iter->cmd, "export"))
			builtin_export(iter, &data->variables, envp, data);
		else if (ft_strlen(iter->cmd) && check_builtin_name(iter->cmd, "unset"))
			builtin_unset(iter, &data->variables, envp);
		else if (ft_strlen(iter->cmd) && check_builtin_name(iter->cmd, "cd"))
			builtin_cd(iter, data->variables);
		else if (iter->cmd[0] == '.')
			execute(iter, *envp, act, data);
		else if (find_path(iter->cmd, *envp))
			(execute(iter, *envp, act, data)); //segundo EXECUTE
		iter = iter->next;
	}
}
