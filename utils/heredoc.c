/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 21:22:55 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/20 11:31:03 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	mini_gnl(char **line)
{
	char	*buffer;
	int		i;
	int		r;
	char	c;

	i = 0;
	r = 0;
	buffer = (char *)malloc(10000);
	if (!buffer)
		return (-1);
	r = read(0, &c, 1);
	while (r && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		r = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = buffer;
	return (r);
}

void	heredoc_child(S_SIG **act, int *fd, int pid)
{
	int	status;

	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	waitpid(pid, &status, 0);
	if (g_reset_fd[2] != 130 && g_reset_fd[2] != 131)
		g_reset_fd[2] = WEXITSTATUS(status);
	config_sigaction((void *)act, SIG_IGN, SIGQUIT);
	config_sigaction((void *)act, sigint_handle, SIGINT);
}

void	config_signals(S_SIG **act)
{
	config_sigaction((void *)act, handle_sigquit, SIGQUIT);
	config_sigaction((void *)act, sigint_handle_cmd, SIGINT);
}

void	here_doc(char *limiter, S_SIG **act)
{
	pid_t	pid;
	int		fd[2];
	char	*line;

	config_signals(act);
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		config_sigaction((void *)act, handle_heredoc, SIGINT);
		while (mini_gnl(&line))
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
			{
				free(line);
				break ;
			}
			write(fd[1], line, ft_strlen(line));
			free(line);
		}
		exit(errno);
	}
	else
		heredoc_child(act, fd, pid);
}

// NECESSÁRIO ADAPTAR PARA FORMATO DATA
