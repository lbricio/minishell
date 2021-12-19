/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:18:59 by felipe            #+#    #+#             */
/*   Updated: 2021/12/19 19:53:01 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	len_list(t_args *list)
{
	t_args	*iter;
	int		i;

	i = 0;
	iter = list;
	while (iter)
	{
		i++;
		iter = iter->next;
	}
	return (i);
}

void	free_paths(char **paths)
{
	int	i;

	i = -1;
	while (paths[++i] != 0)
		free(paths[i]);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = -1;
	while (envp[++i])
	{
		if (ft_strnstr(envp[i], "PATH", 4) != 0)
			break ;
	}
	if (envp[i])
	{
		paths = ft_split(envp[i] + 5, ':');
		i = 0;
		while (paths[i])
		{
			part_path = ft_strjoin(paths[i], "/");
			path = ft_strjoin(part_path, cmd);
			free(part_path);
			if (access(path, F_OK) == 0)
			{
				free_paths(paths);
				free(paths);
				return (path);
			}
			free(path);
			i++;
		}
		free_paths(paths);
		free(paths);
	}
	return (0);
}

char	*change_directory(char *path)
{
	char	*slash;
	char	*final;
	char	*file;

	slash = ft_strchr(path, '/');
	while (slash)
	{
		final = slash;
		slash = ft_strchr(slash + 1, '/');
	}
	file = final + 1;
	*final = 0;
	chdir(path);
	*final = '/';
	return (file);
}

int	open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (i == 2)
		file = open(argv, O_RDONLY, 0777);
	return (file);
}

void	run_aux(t_cmds *cmds, pid_t pid, int *fd, S_SIG **act)
{
	int	status;

	close(fd[1]);
	if (cmds->fd_in > 0)
		reset_input();
	if (cmds->fd_out == 1000)
		dup2(fd[0], STDIN_FILENO);
	waitpid(pid, &status, 0);
	if (g_reset_fd[2] != 130 && g_reset_fd[2] != 131)
		g_reset_fd[2] = WEXITSTATUS(status);
	close(fd[0]);
	if (cmds->fd_out != 1000 && cmds->fd_out != 0)
	{
		reset_input();
		reset_output();
	}
	if (cmds->fd_out == 0)
		reset_input();
	config_sigaction((void *)act, SIG_IGN, SIGQUIT);
	config_sigaction((void *)act, sigint_handle, SIGINT);
}

void	run(char *file_path, char **argv, char **envp, t_cmds *cmds, S_SIG **act)
{
	pid_t	pid;
	int		fd[2];

	config_sigaction((void *)act, handle_sigquit, SIGQUIT);
	config_sigaction((void *)act, sigint_handle_cmd, SIGINT);
	pipe(fd);
	pid = fork();
	if(pid == 0)
	{
		if (cmds->fd_in > 0)
			dup2(cmds->fd_in, STDIN_FILENO);
		close(fd[0]);
		if (cmds->fd_out == 0)
			reset_output();
		else if (cmds->fd_out == 1000)
			dup2(fd[1], STDOUT_FILENO);
		else
			dup2(cmds->fd_out, STDOUT_FILENO);
		if (execve(file_path, argv, envp) == -1)
			no_file(file_path);
		exit(errno);
	}
	else
		run_aux(cmds, pid, fd, act);
}

/* Function that take the command and send it to find_path
 before executing it. */
int	execute(t_cmds *cmds, char **envp, S_SIG **act, t_data *data)
{
	t_args	*iter;
	char	**argv;
	char	*path;
	int		i;
	int		size;

	size = 1;
	if (cmds->flags)
		size++;
	size += len_list(cmds->args);
	argv = ft_calloc(size + 1, sizeof(char *));
	if (!argv)
		cleanup(data, 2);
	argv[0] = ft_strdup(cmds->cmd);
	if (!argv[0])
	{
		free(argv);
		cleanup(data, 2);
	}
	i = 1;
	iter = cmds->args;
	if(cmds->flags)
	{
		argv[i] = ft_strdup(cmds->flags);
		if (!argv[i - 1])
		{
			while (--i >= 0)
				free(argv[i]);
			cleanup(data, 2);
		}
		i++;
	}
	while (iter)
	{
		argv[i++] = iter->arg;
		iter = iter->next;
	}
	path = find_path(cmds->cmd, envp);
	if (access(cmds->cmd, X_OK) == 0)
		run(cmds->cmd, argv, envp, cmds, act);
	else if (path && cmds->cmd[0] != '.')
		run(path, argv, envp, cmds, act);
	else if (access(cmds->cmd, F_OK) == -1)
	{
		i = -1;
		while (argv[++i] != 0)
			free(argv[i]);
		free(argv);
		free(path);
		return(exec_no_file(cmds));
	}
	else
	{
		i = -1;
		while (argv[++i] != 0)
			free(argv[i]);
		free(argv);
		free(path);
		return(exec_no_perm(cmds));
	}
	free(path);
	free(argv[0]);
	if (cmds->flags)
		free(argv[1]);
	free(argv);
	return (1);
}
