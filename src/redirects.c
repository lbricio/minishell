/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:30 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 19:24:09 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// transforma < e seus argumentos em whitespaces
static char	*remove_input_char(char *line)
{
	int	i;

	i = 0;
	while (line[i])
		i++;
	while (line[i] != '<')
		i--;
	if (i > 0 && line[i - 1] == '<')
		i--;
	while (line[i] == '<')
	{
		line[i] = ' ';
		i++;
	}
	while (line[i] == ' ')
		i++;
	while ((line[i] >= 'a' && line[i] <= 'z') \
	|| (line[i] >= 'A' && line[i] <= 'Z') \
	|| (line[i] >= '0' && line[i] <= '9') \
	|| line[i] == '.')
	{
		line[i] = ' ';
		i++;
	}
	return (line);
}

static char	*treat_input_utils(char *line, int i, t_cmds *cmds)
{
	char	*outfile;

	while (!((line[i] >= 'a' && line[i] <= 'z') \
	|| (line[i] >= 'A' && line[i] <= 'Z') \
	|| (line[i] >= '0' && line[i] <= '9') \
	|| line[i] == '.'))
		i++;
	outfile = 0;
	outfile = ft_strword(line + i, outfile);
	cmds->fd_in = open_file(outfile, 2);
	if (cmds->fd_in == -1)
	{
		no_file(outfile);
		write(1, "\n", 1);
		g_reset_fd[2] = 127;
		free(outfile);
		return ("");
	}
	free(outfile);
	return (0);
}

// procura por < e <<, trata o input, transforma </<< e seus argumentos
// em whitespaces
char	*treat_input_red(char *line, t_cmds *cmds, t_sig **act)
{
	char	*limiter;
	int		i;

	i = 0;
	while (line[i])
		i++;
	while (line[i] != '<')
		i--;
	if (i > 0 && line[i - 1] == '<')
	{
		while (line[i] == '<' || line[i] == ' ')
			i++;
		limiter = 0;
		limiter = ft_strword(line + i, limiter);
		here_doc(limiter, act);
		free(limiter);
	}
	else
		if (treat_input_utils(line, i, cmds) != 0)
			return ("");
	return (remove_input_char(&line[0]));
}

static void	get_redirect_utils(char *line, t_cmds *cmds, int o_file, int *count)
{
	char	*outfile;
	int		i;

	i = 0;
	while (line[i] == '>' || line[i] == ' ')
		i++;
	outfile = 0;
	outfile = ft_strword(line + i, outfile);
	cmds->fd_out = open_file(outfile, o_file);
	free(outfile);
	while ((line[i] >= 'a' && line[i] <= 'z') \
	|| (line[i] >= 'A' && line[i] <= 'Z') \
	|| (line[i] >= '0' && line[i] <= '9') \
	|| line[i] == '.')
		i++;
	(*count) += i;
}

void	get_redirect(char *line, int *count, t_cmds *cmds)
{
	while (*line == ' ')
		line++;
	if (line[0] == '|' && line[1] != '|')
		cmds->fd_out = 1000;
	else if (line[0] == '>' && line[1] == '>' && line[2] != '>')
		get_redirect_utils(line, cmds, 0, count);
	else if (line[0] == '>' && line[1] != '>' && line[2] != '>')
		get_redirect_utils(line, cmds, 1, count);
	else
		cmds->fd_out = 1;
}
