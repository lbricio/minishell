/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:04:50 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 18:06:24 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>

# define S_SIG struct sigaction

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
	int			fd_in;
	int			fd_out;
}	t_cmds;

typedef struct variables
{
	int					is_env;
	char				*var;
	char				*value;
	struct variables	*next;
}	t_vars;

extern int g_reset_fd[3];

void	save_env_var(char *line, int *count, t_vars **variables);
void	exec_builtin(t_cmds *cmds, t_vars **variables, char ***envp, S_SIG **act);
void	substitute_variables(char **line, t_vars *variables);
void	lstadd_back(t_vars **lst, t_vars *new);
void	*ft_calloc(size_t nmemb, size_t size);
void	builtin_red(t_cmds  *cmds, S_SIG **act, int builtin, char **envp);
void	save_origin_fd();
void	reset_input();
void	reset_output();
void	handle_sigquit(int sig);
void	sigint_handle_cmd(int sig);
void	sigint_handle(int sig);
void	config_sigaction(S_SIG *act, void (*handler)(int), int sig);
void	handle_heredoc(int sig_num);
char	*ft_strnstr(const char	*big, const char *little, size_t len);
char	*get_variable(char *line, int size, t_vars *variables);
char	**ft_split(char const *s, char c);
char	*find_path(char *cmd, char **envp);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strndup(const char *s, int len);
char	*cmds_to_string(t_cmds *cmds);
char	*ft_itoa(int n);
char	*get_prompt();
char	*status_itoa();
char	*ft_strword(const char *s);
int		*parser(char *line, t_vars **variables, char ***envp, S_SIG **act);
int		builtin_export(t_cmds *cmds, t_vars **variables, char ***envp);
int		builtin_unset(t_cmds *cmds, t_vars **variables, char ***envp);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		builtin_exit(t_cmds *cmds, t_vars *variables);
int		check_cmds(t_cmds *cmds, char **envp, S_SIG **act);
int		check_unspecified_chars(char *line);
int		execute(t_cmds *cmds, char **envp, S_SIG **act);
int		check_quotation(char *line);
int		ft_atoi(const char *str);
int		open_file(char *argv, int i);
int		builtin_cd(t_cmds *cmds, t_vars *variables);
void	builtin_pwd(t_cmds  *cmds);
void	builtin_env(char **envp, t_cmds *cmds);
int		no_file(char *file);
int		sintax_error(void);
char 	*ft_getinput(char *line, t_cmds *cmds);
int		ft_strlen(char *str);
char	*ft_strdup(const char *s);
char	*ft_concat(char **dest, char *src);
char	*ft_strchr(const char *s, int c);

#endif
