/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:02:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/19 19:14:49 by lbricio-         ###   ########.fr       */
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

int	read_lines(char **line, t_data *data, char ***envp, S_SIG **act)
{
	substitute_variables(line, data);
	parser(*line, data, envp, act);
	free(*line);
	reset_input();
	reset_output();
	return (1);
}

void	initialize_vars(t_data *data, char **envp)
{
	int	count;
	int	i;

	save_origin_fd();
	count = 0;
	i = -1;
	while (envp[++i] != 0)
		save_env_var(envp[i], &count, data, 1);
}

char	**copy_envp(char **envp)
{
	char	**new;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	new = ft_calloc(i + 1, sizeof (char *));
	if (!new)
		return (0);
	i = -1;
	while (envp[++i])
	{
		new[i] = ft_strdup(envp[i]);
		if (!new[i])
		{
			while (--i > 0)
				free(new[i]);
			return (0);
		}
	}
	return (new);
}

int	main(int argc, char *argv[], char **envp)
{
	t_data	data;
	char	*line;
	S_SIG	act;
	S_SIG	act_quit;

	data.variables = 0;
	data.envp = copy_envp(envp);
	if (!data.envp)
		return (1);
	initialize_vars(&data, envp);
	/* t_vars *iter;
	iter = data.variables;
	while (iter)
	{
		printf("%s=%s\n", iter->var, iter->value);
		iter = iter->next;
	} */
	config_sigaction(&act, sigint_handle, SIGINT);
	config_sigaction(&act_quit, SIG_IGN, SIGQUIT);
	while (1)
	{
		line = readline("\001\033[1;33m\002Minishell> \001\033[0m\002");
		if (!line)
		{
			data.cmds = 0;
			cleanup(&data, 1);
			break ;
		}
		if (line[0] != 0)
		{
			add_history(line);
			if (check_quotation(line, &data))
			{
				printf("Unclosed quotation\n");
				free(line);
			}
			else if (check_unspecified_chars(line, &data))
				free(line);
			else if (!read_lines(&line, &data, &data.envp, (void *)&act))
				return (0);
		}
		else
			free(line);
		cleanup(&data, 0);
	}
	rl_clear_history();
	(void)argc;
	(void)argv;
}
