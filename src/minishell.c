/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:02:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/10 14:56:53 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*get_prompt()
{
	char	path[500];
	char	*prompt;

	getcwd(path, 500);
	prompt = ft_strjoin(path, "# ");
	return (prompt);
}

int	read_lines(char **line, t_vars **variables, char **envp)
{
	substitute_variables(line, *variables);
	while(parser(*line, variables, envp));
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

int	main(int argc, char *argv[], char **envp)
{
	t_vars	*variables;
	t_cmds	*cmds;
	char	*line;
	char	*prompt;
	
	initialize_vars(&variables, envp);
	recieve_signals();
	while (1)
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
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
			else if (!read_lines(&line, &variables, envp))
				return (0);
		}
		else
			free(line);
	}
	rl_clear_history();
}
