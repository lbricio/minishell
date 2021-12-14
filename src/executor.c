/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:19:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/14 13:02:16 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_util(t_cmds *iter, t_data *data, char ***envp, S_SIG **act)
{
	if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "env", \
	ft_strlen(iter->cmd)))
		builtin_red(data->cmds, act, 3, *envp);
	else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, \
	"echo", ft_strlen(iter->cmd)))
		builtin_red(data->cmds, act, 1, *envp);
	else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, \
	"pwd", ft_strlen(iter->cmd)))
		builtin_red(data->cmds, act, 2, *envp);
	else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, \
	"exit", ft_strlen(iter->cmd)))
		builtin_exit(iter, data);
	else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, \
	"export", ft_strlen(iter->cmd)))
		builtin_export(iter, data, envp);
	else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, \
	"unset", ft_strlen(iter->cmd)))
		builtin_unset(iter, &data->variables, envp);
	else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, \
	"cd", ft_strlen(iter->cmd)))
		builtin_cd(iter, data->variables);
	else if (iter->cmd[0] == '.')
		execute(iter, *envp, act);
	else if (find_path(iter->cmd, *envp))
		(execute(iter, *envp, act));
}

void	exec_builtin(t_cmds *cmds, t_data *data, char ***envp, S_SIG **act)
{
	t_cmds	*iter;
	char	*line;

	iter = cmds;
	while (iter != 0)
	{
		exec_util(iter, data, envp, act);
		iter = iter->next;
	}
}
