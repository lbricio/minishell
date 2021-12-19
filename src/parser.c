/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:09:19 by felipe            #+#    #+#             */
/*   Updated: 2021/12/19 14:29:58 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* funcao para inicializar todos as variaveis da struct t_cmds */
void	init_cmds(t_cmds *cmds)
{
	cmds->cmd = 0;
	cmds->flags = 0;
	cmds->args = 0;
	cmds->fd_in = 0;
	cmds->fd_out = 0;
	cmds->next = 0;
}

/* funcao para remover um char. Usada para remover as aspas */
void remove_char(char *s, char c)
{
	int	writer;
	int	reader;

	writer = 0;
	reader = 0;
	while (s[reader] != 0)
	{
		if (s[reader] != c)
			s[writer++] = s[reader];
		reader++;       
	}
	s[writer]=0;
}

/* retorna o primeiro tipo de aspas encontrada */
char	get_quote(char *line)
{
	int	i;

	i = -1;
	while (line[++i] != 0)
	{
		if (line[i] == '"')
			return ('"');
		if (line[i] == '\'')
			return ('\'');
	}
	return (0);
}

/* funçao para retornar um comando como string */
char	*get_cmd(char *line, int *count, t_data *data)
{
	char	*cmd;
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while ((line[i] != 0 && line[i] != ' ' && line[i] != '|' && line[i] != ';') || quote)
	{
		if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	cmd = ft_strndup(line, i);
	if (!cmd)
		cleanup(data, 2);
	quote = get_quote(cmd);
	remove_char(cmd, quote);
	(*count) += i;
	return (cmd);
}

char 	*trunc_input_filename(char *line)
{
	char *new_str;
	char *newest_str;
	int i;
	int size;

	i = 1;
	size = 0;
	new_str = malloc(40 * sizeof(char));
	while (line[i] != 0 && ((line[i] >= 'a' && line[i] <= 'z') 
		|| (line[i] >= 'A' && line[i] <= 'Z')
		|| (line[i] >= '0' && line[i] <= '9')))
		{
			new_str[size] = line[i];
			size++;
			i++;
		}
	new_str[++size] = '\0';
	newest_str = ft_strdup(new_str);
	free(new_str);
	return (newest_str);
}

// translate -l -h -a into -lha
char 	*trunc_flags(char *flags, t_data *data)
{
	char *new_str;
	char *newest_str;
	int i;
	int size;
	
	new_str = ft_calloc(ft_strlen(flags) + 1, sizeof(char));
	if (!new_str)
		cleanup(data, 2);
	if(flags[0] != '-')
		return (0);
	i = 1;
	size = 0;
	new_str[0] = '-';
	while(flags[i] != 0)
	{
		if((flags[i] >= 'a' && flags[i] <= 'z') || (flags[i] >= 'A' && flags[i] <= 'Z'))
			new_str[++size] = flags[i];
		i++;
	}		
	new_str[++size] = '\0';
	newest_str = ft_strdup(new_str);
	free(new_str);
	if (!newest_str)
		cleanup(data, 2);
	return(newest_str);
}

/* retorna as flags como uma string */
char	*get_flags(char *line, int *count, t_data *data)
{
	char	*flags;
	char	*trunc;
	char	quote;
	int		i;

	if (line[0] == '-' || ((line[0] == '"' || line[0] == '\'') && line[1] == '-'))
	{
		quote = 0;
		i = 0;
		while (line[i] != 0 && line[i] != ' ' && line[i] != '|' && line[i] != ';')
		{
			if ((line[i] == '\'' || line[i] == '"') && !quote)
				quote = line[i];
			else if (line[i] == quote)
				quote = 0;
			i++;
			if(line[i] == ' ')
				if(line[i + 1] == '-')
					i += 2;
		}
		flags = ft_strndup(line, i);
		if (!flags)
			cleanup(data, 2);
		quote = get_quote(flags);
		remove_char(flags, quote);
		(*count) += i;
		trunc = trunc_flags(flags, data);
		free(flags);
		return (trunc);
	}
	return (0);
}

/* retorna os argumentos em uma lista */
t_args	*get_args(char *line, int *count, t_data *data)
{
	t_args	*args;
	t_args	*iter;
	char	quote;
	int		i;
	int		j;

	args = ft_calloc(1, sizeof (t_args));
	if (!args)
		cleanup(data, 2);
	args->arg = 0;
	args->next = 0;
	iter = args;
	i = 0;
	while (line[i] != 0 && line[i] != '|' && line[i] != ';' && line[i] != '>')
	{
		quote = 0;
		j = 0;
		while ((line[i + j] != 0 && line[i + j] != '|' && line[i + j] != ';' && line[i + j] != '>' && line[i + j] != ' ') || quote)
		{
			if ((line[i + j] == '\'' || line[i + j] == '"') && !quote)
				quote = line[i + j];
			else if (line[i + j] == quote)
				quote = 0;
			j++;
		}
		iter->arg = ft_strndup(line + i, j);
		if (!iter->arg)
			cleanup(data, 2);
		iter->next = 0;
		quote = get_quote(iter->arg);
		if (quote)
			remove_char(iter->arg, quote);
		while (line[i + j] == ' ')
			j++;
		//printf("[arg:%s]\n",iter->arg);
		if (line[i + j] != 0 && line[i + j] != '|' && line[i + j] != ';')
		{
			iter->next = ft_calloc(1, sizeof (t_args));
			if (!iter->next)
				cleanup(data, 2);
			iter = iter->next;
			iter->arg = 0;
			iter->next = 0;
		}
		i += j;
	}
	(*count) += i;
	return (args);
}

void	add_variable(t_data *data, t_vars *new)
{
	t_vars	*iter;
	int		size_new;

	size_new = ft_strlen(new->var);
	iter = data->variables;
	while (iter)
	{
		if (!ft_strncmp(iter->var, new->var, size_new) && size_new == ft_strlen(iter->var))
		{
			free(iter->value);
			iter->value = new->value;
			free(new->var);
			free(new);
			return ;
		}
		iter = iter->next;
	}
	lstadd_back(data, new);
}

void	save_env_var(char *line, int *count, t_data *data, int env)
{
	t_vars	*new;
	char	quote;
	int		quote_count;
	int		equal;
	int		end;
	int		i;

	if (line[0] == '"' || line[0] == '\'')
		return ;
	end = 0;
	equal = 0;
	quote = get_quote(line);
	quote_count = 0;
	i = -1;
	while (line[++i] != 0 && ((line[i] != ';' && line[i] != '|') || env))
	{
		if (line[i] == '=')
			equal = i;
		else if (line[i] == ' ' && equal == 0)
			return ;
		else if (line[i] == ' ' && (!quote || quote_count == 2) && !env)
			break ;
		else if (line[i] == quote)
			quote_count++;
	}
	end = i;
	while (line[i] == ' ')
		i++;
	if (equal)
		(*count) += end;
	if (line[i] != 0 && line[i] != ';')
		return ;
	if (equal)
	{
		new = ft_calloc(1, sizeof (t_vars));
		if (!new)
			cleanup(data, 2);
		new->value = ft_strndup(line + equal + 1, end - equal - 1);
		if (!new->value)
			cleanup(data, 2);
		remove_char(new->value, '"');
		remove_char(new->value, '\'');
		new->var = ft_strndup(line, equal);
		if (!new->var)
			cleanup(data, 2);
		new->next = 0;
		add_variable(data, new);
	}
}

int		sintax_check_aux(char *line, int i)
{
	i++;
	while(line[i] == ' ')
		i++;
	if (line[i] == '|' || line[i] == '>' || line[i] == '<' || line[i] == '\0' )
	{
		if(!((line [i] == '>' && line [i - 1] == '>') 
			|| (line [i] == '<' && line [i - 1] == '<')))
			{
				while(line[i] == ' ')
					i++;
				if(!((line[i] >= 'a' && line[i] <= 'z')
					|| (line[i] >= 'A' && line[i] <= 'Z')
					|| (line[i] >= '0' && line[i] <= '9')))
						return (-1);
			}
	}
		else
		{
				while(line[i] == ' ' || line[i] == '>' || line[i] == '<')
					i++;
				if(!((line[i] >= 'a' && line[i] <= 'z')
					|| (line[i] >= 'A' && line[i] <= 'Z')
					|| (line[i] >= '0' && line[i] <= '9')))
						return (-1);
		}
	return (0);
}

int			sintax_check(char *line)
{
	int i;
	int error;

	i = 0;
	error = 0;
	if (line[i] == '|')
		error = -1;
	while (line[i])
	{
		if (line[i] == '|' || line[i] == '>' || line[i] == '<')
			error = sintax_check_aux(line, i);
		i++;
	}
	if (error == -1)
		return (sintax_error());
	return (0);
}

// transforma < e seus argumentos em whitespaces
char	*remove_input_char(char *line)
{
	int i;

	i = 0;
	while(line[i])
		i++;
	while(line[i] != '<')
		i--;
	if(line[i - 1] == '<')
		i--;
	while (line[i] == '<')
	{
		line[i] = ' ';
		i++;
	}
	while (line[i] == ' ')
		i++;
	while ((line[i] >= 'a' && line[i] <= 'z')
	|| (line[i] >= 'A' && line[i] <= 'Z')
	|| (line[i] >= '0' && line[i] <= '9'))
	{
		line[i] = ' ';
		i++;
	}
	return(line);
}

// procura por < e <<, trata o input, transforma </<< e seus argumentos em whitespaces
char 	*treat_input_red(char *line, t_cmds *cmds, S_SIG **act)
{
	char *outfile;
	char *limiter;
	int i;

	i = 0;
	while(line[i])
		i++;
	while(line[i] != '<')
		i--;
	if(line[i - 1] == '<')
	{
		while(line[i] == '<' || line[i] == ' ')
			i++;
		limiter = ft_strword(line + i, limiter);
		here_doc(limiter, act);
		free(limiter);
	}
	else
	{
		while (!((line[i] >= 'a' && line[i] <= 'z')
		|| (line[i] >= 'A' && line[i] <= 'Z')
		|| (line[i] >= '0' && line[i] <= '9')))
			i++;
		outfile = ft_strword(line + i, outfile);
		free(outfile);
		cmds->fd_in = open_file(outfile, 2);
		if (cmds->fd_in == -1)
		{
			no_file(outfile);
			write(1, "\n", 1);
			g_reset_fd[2] = 127;
			return ("");
		}
		
	}
	return(remove_input_char(&line[0]));
}

void		get_redirect(char *line, int *count, t_cmds *cmds, t_data *data)
{
	char 	*outfile;
	int		i;

	if (line[0] == '|' && line[1] != '|')
		cmds->fd_out = 1000;
	else if (line[0] == '>' && line[1] == '>' && line[2] != '>')
	{
		i = 0;
		while (line [i] == '>' || line[i] == ' ')
			i++;
		outfile = ft_strword(line + i, outfile);
		free(outfile);
		cmds->fd_out = open_file(outfile, 0);
		while ((line[i] >= 'a' && line[i] <= 'z') 
		|| (line[i] >= 'A' && line[i] <= 'Z')
		|| (line[i] >= '0' && line[i] <= '9'))
			i++;
		(*count) += i;
	}
	else if (line[0] == '>' && line[1] != '>' && line[2] != '>')
	{
		i = 0;
		while (line [i] == '>' || line[i] == ' ')
			i++;
		outfile = ft_strword(line + i, outfile);
		free(outfile);
		cmds->fd_out = open_file(outfile, 1);
		while ((line[i] >= 'a' && line[i] <= 'z')
		|| (line[i] >= 'A' && line[i] <= 'Z')
		|| (line[i] >= '0' && line[i] <= '9'))
			i++;
		(*count) += i;
	}
	else
		cmds->fd_out = 0;
}

int		*parser(char *line, t_data *data, char ***envp, S_SIG **act)
{
	t_cmds	*iter;
	int		j;

	data->cmds = malloc(sizeof (t_cmds));
	if (!data->cmds)
		cleanup(data, 2);
	init_cmds(data->cmds);
	iter = data->cmds;
	j = 0;
	while (line[j] != 0 && line[j] != ';')
	{
		while (line[j] == ' ')
			j++;
		if (get_quote(line) == 0)
		{
			if (sintax_check(line + j) == -1)
				break ;
			iter->fd_in = 0;
			if(strchr(line, '<'))
			{
				line = ft_strdup(treat_input_red(line + j, iter, act));
				if (!line)
					cleanup(data, 2);
			}
		}
		save_env_var(line + j, &j, data, 0);
		while (line[j] == ' ')
			j++;
		iter->cmd = get_cmd(line + j, &j, data);
		remove_char(iter->cmd, get_quote(iter->cmd));
		while (line[j] == ' ')
			j++;
		iter->flags = get_flags(line + j, &j, data);
		while (line[j] == ' ')
			j++;
		iter->args = get_args(line + j, &j, data);
		while (line[j] == ' ')
			j++;
		get_redirect(line + j, &j, iter, data);
		while (line[j] == ' ')
			j++;
		if (iter->cmd[0] != '\0')
			if ((check_cmds(iter, *envp, act, data))) // 0 = executou
				exec_builtin(iter, data, envp, act);
		if (line[j] == '|' && line[j + 1] != '|')
		{
			iter->next = malloc(sizeof (t_cmds));
			if (!iter->next)
				cleanup(data, 2);
			iter = iter->next;
			init_cmds(iter);
			j++;
		}
	}	
	return (0);
}
