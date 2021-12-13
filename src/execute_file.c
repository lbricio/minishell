/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:18:59 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 18:59:17 by lbricio-         ###   ########.fr       */
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
}

/* Function that will look for the path line inside the environment, will
 split and test each command path and then return the right one. */
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
			break;
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
				return (path);
			i++;
		}
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

void	error(void)
{
	perror("\033[31mError");
	exit(EXIT_FAILURE);
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
	if (file == -1)
		error();
	return (file);
}

void	run(char *file_path, char **argv, char **envp, t_cmds *cmds, S_SIG **act)
{
	pid_t	pid;
	int		fd[2];
	int		status;
	pipe(fd);
	pid = fork();
	if(pid == 0)
	{
		config_sigaction((void *)act, handle_sigquit, SIGQUIT);
		close(fd[0]);
		if (cmds->fd_out == 0)
			reset_output();
		else if (cmds->fd_out == 1000)
			dup2(fd[1], STDOUT_FILENO);
		else
			dup2(cmds->fd_out, STDOUT_FILENO);
		if (execve(file_path, argv, envp) == -1);
			no_file(file_path);
		exit(errno);
	}
	else
	{
		close(fd[1]);
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
		//printf("exit code:%i\n",g_reset_fd[2]);
	}
}

/* Function that take the command and send it to find_path
 before executing it. */
int	execute(t_cmds *cmds, char **envp, S_SIG **act)
{
	t_vars	*v_iter;
	t_args	*iter;
	char	**argv;
	int		i;

	argv = ft_calloc(10, sizeof(char *));
	argv[0] = ft_strdup(cmds->cmd);
	i = 1;
	iter = cmds->args;
	if(cmds->flags)
		argv[i++] = ft_strdup(cmds->flags);
	while (iter)
	{
		argv[i] = iter->arg;
		i++;
		iter = iter->next;
	}
	if (access(cmds->cmd, X_OK) == 0)
		run(cmds->cmd, argv, envp, cmds, act);
	else if (find_path(cmds->cmd, envp) && cmds->cmd[0] != '.')
		run(find_path(cmds->cmd, envp), argv, envp, cmds, act);
	else if (access(cmds->cmd, F_OK) == -1)
	{
		printf("%s: No such file or directory\n", cmds->cmd);
		reset_input();
		reset_output();
		g_reset_fd[2] = 127;
		return (0);
	}
	else
	{
		printf("%s: Permission denied\n", cmds->cmd);
		reset_input();
		reset_output();
		g_reset_fd[2] = 126;
		return (0);
	}
	return (1);
}
