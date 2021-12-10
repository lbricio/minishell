/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:04:50 by felipe            #+#    #+#             */
/*   Updated: 2021/12/10 15:42:56 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include "gnl/get_next_line.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>

# define HISTFILESIZE
# define ECHO 0
# define ECHO_OPT 1
# define CD 2
# define PWD 3
# define EXPORT 4
# define UNSET 5
# define ENV 6
# define EXIT 7

typedef struct args
{
	char		*arg;
	struct args	*next;
}	t_args;

typedef struct cmds
{
	char		*cmd;
	char		*flags;
	t_args		*args;
	struct cmds	*next;
	int			fd_out;
}	t_cmds;

typedef struct variables
{
	char				*var;
	char				*value;
	struct variables	*next;
}	t_vars;

int g_reset_fd[3];

int		*parser(char *line, t_vars **variables, char **envp);
void	save_env_var(char *line, int *count, t_vars **variables);
void	executor(t_cmds *cmds, t_vars *variables, char **envp);
void	substitute_variables(char **line, t_vars *variables);
void	lstadd_back(t_vars **lst, t_vars *new);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_echo(t_cmds *iter);
void	recieve_signals(void);
char	*ft_strnstr(const char	*big, const char *little, size_t len);
char	*get_variable(char *line, int size, t_vars *variables);
char	**ft_split(char const *s, char c);
char	*find_path(char *cmd, char **envp);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strndup(const char *s, int len);
char	*cmds_to_string(t_cmds *cmds);
char	*get_prompt();
char	*status_itoa();
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		builtin_exit(t_cmds *cmds, t_vars *variables);
int		check_cmds(t_cmds *cmds, char **envp);
int		check_unspecified_chars(char *line);
int		execute(t_cmds *cmds, char **envp);
int		check_quotation(char *line);
int		ft_atoi(const char *str);
int		open_file(char *argv, int i);
char	*ft_strword(const char *s);
void	save_origin_fd();
void	reset_input();
void	reset_output();
#endif
