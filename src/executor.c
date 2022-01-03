/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:18:39 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:26:18 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_builtin(t_cmds *cmds, t_data *data, char ***envp, t_sig **act)
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
			(execute(iter, *envp, act, data));
		iter = iter->next;
	}
}

void	executor(t_data *data, char ***envp, t_sig **act)
{
	t_cmds	*iter;
	char	*path;

	iter = data->cmds;
	while (iter)
	{
		path = find_path(iter->cmd, *envp);
		if (!is_builtin(iter->cmd) && (iter->cmd[0] == '.' \
		|| iter->cmd[0] == '~' || iter->cmd[0] == '/' || path) \
		&& iter->cmd[0] != 0)
		{
			g_reset_fd[2] = 0;
			execute(iter, *envp, act, data);
		}
		else if (iter->cmd[0] != 0)
			exec_builtin(iter, data, envp, act);
		iter = iter->next;
		free(path);
	}
	reset_input();
	reset_output();
}
