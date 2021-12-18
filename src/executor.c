/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:19:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/18 16:55:41 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_builtin(t_cmds *cmds, t_data *data, char ***envp, S_SIG **act)
{
	t_cmds	*iter;

	iter = cmds;
	while (iter != 0)
	{
		if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "env", 3))
			builtin_red(cmds, act, 3, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "echo", 4))
			builtin_red(cmds, act, 1, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "pwd", 3))
			builtin_red(cmds, act, 2, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "exit", 4))
			builtin_exit(iter, data);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "export", 6))
			builtin_export(iter, &data->variables, envp, data);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "unset", 5))
			builtin_unset(iter, &data->variables, envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "cd", 2))
			builtin_cd(iter, data->variables);
		else if (iter->cmd[0] == '.')
			execute(iter, *envp, act, data);
		else if (find_path(iter->cmd, *envp))
			(execute(iter, *envp, act, data)); //segundo EXECUTE
		iter = iter->next;
	}
}
