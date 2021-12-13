/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:02:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 17:57:16 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_reset_fd[3];

int	str_to_cmd(char *str, int *j)
{
	int	len;
	int	cmd;

	cmd = -1;
	len = 0;
	while (str[len + *j] != ' ' && str[len + *j] != '|' && str[len + *j] != 0)
		len++;
	if (!ft_strncmp(str + *j, "echo", len))
		cmd = ECHO;
	else if (!ft_strncmp(str + *j, "cd", len))
		cmd = CD;
	else if (!ft_strncmp(str + *j, "pwd", len))
		cmd = PWD;
	else if (!ft_strncmp(str + *j, "export", len))
		cmd = EXPORT;
	else if (!ft_strncmp(str + *j, "unset", len))
		cmd = UNSET;
	else if (!ft_strncmp(str + *j, "env", len))
		cmd = ENV;
	else if (!ft_strncmp(str + *j, "exit", len))
		cmd = EXIT;
	(*j) += len;
	return (cmd);
}

int	read_lines(char **line, t_vars **variables, char ***envp, S_SIG **act)
{
	substitute_variables(line, *variables);
	parser(*line, variables, envp, act);
	free(*line);
	return (1);
}

t_vars	*initialize_vars(t_vars **variables, char **envp)
{
	int	count;
	int	i;

	i = -1;
	count = 0;
	save_origin_fd();
	while (envp[++i] != 0)
		save_env_var(envp[i], &count, variables);
}

char	**copy_envp(char **envp)
{
	char	**new;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	new = ft_calloc(i + 1, sizeof (char *));
	i = -1;
	while (envp[++i])
		new[i] = ft_strdup(envp[i]);
	return (new);
}

int	main(int argc, char *argv[], char **envp)
{
	t_vars	*variables;
	t_cmds	*cmds;
	char	*line;
	char	**new_envp;
	S_SIG	act;
	S_SIG	act_quit;

	variables = 0;
	new_envp = copy_envp(envp);
	initialize_vars(&variables, envp);
	config_sigaction(&act, sigint_handle, SIGINT);
	config_sigaction(&act_quit, SIG_IGN, SIGQUIT);
	while (1)
	{
		line = readline("\001\033[1;33m\002Minishell> \001\033[0m\002");
		if (!line)
			break ;
		if (line[0] != 0)
		{
			add_history(line);
			if (check_quotation(line))
			{
				printf("Unclosed quotation\n");
				free(line);
			}
			else if (check_unspecified_chars(line))
				free(line);
			else if (!read_lines(&line, &variables, &new_envp, (void *)&act))
				return (0);
		}
		else
			free(line);
	}
	rl_clear_history();
}
