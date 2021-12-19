/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:11:27 by felipe            #+#    #+#             */
/*   Updated: 2021/12/19 19:49:29 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* funcao para verificar se a flag passada é valida para o comando
 * passado */
int	is_flag(t_cmds *cmds)
{
	if (cmds->flags)
	{
		if (!ft_strncmp(cmds->cmd, "echo", ft_strlen(cmds->cmd)) \
		&& !ft_strncmp(cmds->flags, "-n", ft_strlen(cmds->flags)))
			return (1);
		return (0);
	}
	return (1);
}

// funcao para verificar se o comando passado é um builtin
int	is_builtin(char *cmd)
{
	int	len;

	if (cmd)
	{
		len = ft_strlen(cmd);
		if (!ft_strncmp(cmd, "echo", len))
			return (1);
		else if (!ft_strncmp(cmd, "cd", len))
			return (1);
		else if (!ft_strncmp(cmd, "pwd", len))
			return (1);
		else if (!ft_strncmp(cmd, "export", len))
			return (1);
		else if (!ft_strncmp(cmd, "unset", len))
			return (1);
		else if (!ft_strncmp(cmd, "env", len))
			return (1);
		else if (!ft_strncmp(cmd, "exit", len))
			return (1);
		return (0);
	}
	return (1);
}

/* funcao para verificar se os comandos e as flags existem */
int	check_cmds(t_cmds *cmds, char **envp, S_SIG **act, t_data *data)
{
	t_cmds	*iter;
	char	*path;

	iter = cmds;
	while (iter)
	{
		path = find_path(iter->cmd, envp);
		if (!is_builtin(iter->cmd) && (iter->cmd[0] == '.'
				|| iter->cmd[0] == '~' || iter->cmd[0] == '/' || path))
		{
			g_reset_fd[2] = 0;
			execute(iter, envp, act, data);
			free(path);
			return (0);
		}
		else if (!is_builtin(iter->cmd))
			return (cmd_error(cmds));
		else if (!is_flag(iter))
			return (flag_error(cmds));
		iter = iter->next;
		free(path);
	}
	return (1);
}

/* verifica se há uma quantidade par da primeira aspas encontrada
 * ainda tenho que considerar o caso de encontrar aspas abertas e fechadas
 * de um jeito e depois aspas abertas de outro jeito */
int	check_quotation(char *line, t_data *data)
{
	char	quote;
	int		i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (quote && line[i] == quote)
			quote = 0;
		else if (line[i] == '"')
			quote = '"';
		else if (line[i] == '\'')
			quote = '\'';
	}
	if (quote)
	{
		cleanup(data, 0);
		return (-1);
	}
	return (0);
}

int	check_unspecified_chars(char *line, t_data *data)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (line[i] == '"' && !quote)
			quote = '"';
		else if (line[i] == '\'' && !quote)
			quote = '\'';
		else if (line[i] == quote)
			quote = 0;
		else if (!quote && (line[i] == ';' || line[i] == '\\'))
		{
			cleanup(data, 0);
			printf("syntax error near unexpected token ';'\n");
			return (-1);
		}
	}
	return (0);
}
