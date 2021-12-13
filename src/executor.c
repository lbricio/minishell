/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:19:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 01:38:28 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	executor(t_cmds *cmds, t_vars **variables, char ***envp, S_SIG **act)
{
	t_cmds	*iter;
	t_cmds	*next;
	t_args	*next_arg;
	char	*line;

	next_arg = cmds->args;
	iter = cmds;
	while (iter != 0)
	{
		if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "env", ft_strlen(iter->cmd)))
			builtin_red(cmds, act, 3, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "echo", ft_strlen(iter->cmd)))
			builtin_red(cmds, act, 1, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "pwd", ft_strlen(iter->cmd)))
			builtin_red(cmds, act, 2, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "exit", ft_strlen(iter->cmd)))
			builtin_exit(iter, *variables);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "export", ft_strlen(iter->cmd)))
			builtin_export(iter, variables, envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "unset", ft_strlen(iter->cmd)))
			builtin_unset(iter, variables, envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "cd", ft_strlen(iter->cmd)))
			builtin_cd(iter, *variables);
		else if (iter->cmd[0] == '.')
			execute(iter, *envp, act);
		else if (find_path(iter->cmd, *envp))
			(execute(iter, *envp, act));
		iter = iter->next;
	}
}
