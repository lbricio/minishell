/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:09:19 by felipe            #+#    #+#             */
/*   Updated: 2021/12/13 21:45:02 by felipe           ###   ########.fr       */
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
		cleanup(data, 1);
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
	while(line[i] != 0 && (line[i] >= 'a' && line[i] <= 'z' 
		|| line[i] >= 'A' && line[i] <= 'Z'
		|| line[i] >= '0' && line[i] <= '9'))
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
char 	*trunc_flags(char *flags)
{
	char *new_str;
	char *newest_str;
	int i;
	int size;
	
	new_str = malloc(20 * sizeof(char));
	if (!new_str)
		return (0);
	if(flags[0] != '-')
		return (0);
	i = 1;
	size = 0;
	new_str[0] = '-';
	while(flags[i] != 0)
	{
		if(flags[i] >= 'a' && flags[i] <= 'z' || flags[i] >= 'A' && flags[i] <= 'Z')
			new_str[++size] = flags[i];
		i++;
	}		
	new_str[++size] = '\0';
	newest_str = ft_strdup(new_str);
	if (!newest_str)
		return (0);
	free(new_str);
	return(newest_str);
}

/* retorna as flags como uma string */
char	*get_flags(char *line, int *count, t_data *data)
{
	char	*flags;
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
			cleanup(data, 1);
		quote = get_quote(flags);
		remove_char(flags, quote);
		(*count) += i;
		return (trunc_flags(flags));
	}
	return (0);
}

/* retorna os argumentos em uma lista */
t_args	*get_args(char *line, int *count, t_data *data)
{
	t_args	*args;
	t_args	*iter;
	char	quote;
	int		quote_count;
	int		i;
	int		j;

	args = malloc(sizeof (t_args));
	if (!args)
		cleanup(data, 1);
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
			cleanup(data, 1);
		iter->next = 0;
		quote = get_quote(iter->arg);
		remove_char(iter->arg, quote);
		while (line[i + j] == ' ')
			j++;
		if (line[i + j] != 0 && line[i + j] != '|' && line[i + j] != ';')
		{
			iter->next = malloc(sizeof (t_args));
			if (!iter->next)
				cleanup(data, 1);
			iter = iter->next;
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
		if (!ft_strncmp(iter->var, new->var, size_new))
		{
			free(iter->value);
			iter->value = new->value;
			free(new->var);
			return ;
		}
		iter = iter->next;
	}
	lstadd_back(&data->variables, new);
}

int	save_env_var(char *line, int *count, t_data *data)
{
	t_vars	*new;
	char	quote;
	int		quote_count;
	int		equal;
	int		end;
	int		i;

	if (line[0] == '"' || line[0] == '\'')
		return (1);
	end = 0;
	equal = 0;
	quote = get_quote(line);
	quote_count = 0;
	i = -1;
	while (line[++i] != 0 && line[i] != ';' && line[i] != '|')
	{
		if (line[i] == '=')
			equal = i;
		else if (line[i] == ' ' && equal == 0)
			return (1);
		else if (line[i] == ' ' && (!quote || quote_count == 2))
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
		return (1);
	if (equal)
	{
		new = malloc(sizeof (t_vars));
		if (!new)
			cleanup(data, 1);
		new->value = ft_strndup(line + equal + 1, end - equal - 1);
		if (!new->value)
			cleanup(data, 1);
		remove_char(new->value, '"');
		remove_char(new->value, '\'');
		new->var = ft_strndup(line, equal);
		if (!new->var)
			cleanup(data, 1);
		new->next = 0;
		add_variable(data, new);
	}
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
		{
			i++;
			while(line[i] == ' ')
				i++;
			if (line[i] == '|' || line[i] == '>' || line[i] == '<' || line[i] == '\0' )
				if(!((line [i] == '>' && line [i - 1] == '>') 
					|| (line [i] == '<' && line [i - 1] == '<')))
					{
						while(line[i] == ' ')
							i++;
						if(!( line[i] >= 'a' && line[i] <= 'z' 
							|| line[i] >= 'A' && line[i] <= 'Z'
							|| line[i] >= '0' && line[i] <= '9'))
								error = -1;
					}
				else
						while(line[i] == ' ')
							i++;
						if(!( line[i] >= 'a' && line[i] <= 'z' 
							|| line[i] >= 'A' && line[i] <= 'Z'
							|| line[i] >= '0' && line[i] <= '9'))
								error = -1;
		}
		i++;
	}
	if (error == -1)
		return (sintax_error());
}

// transforma < e seus argumentos em whitespaces
char	*remove_input_char(char *line)
{
	int i;
	int size;

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
	while (line[i] >= 'a' && line[i] <= 'z'
	|| line[i] >= 'A' && line[i] <= 'Z'
	|| line[i] >= '0' && line[i] <= '9')
	{
		line[i] = ' ';
		i++;
	}
	return(line);
}

// procura por < e <<, trata o input, transforma </<< e seus argumentos em whitespaces
char 	*treat_input_red(char *line, t_cmds *cmds)
{
	char *outfile;
	int i;
	i = 0;
	while(line[i])
		i++;
	while(line[i] != '<')
		i--;
	if(line[i - 1] == '<')
		printf("heredoc case\n");
	else
	{
		while (!(line[i] >= 'a' && line[i] <= 'z'
		|| line[i] >= 'A' && line[i] <= 'Z'
		|| line[i] >= '0' && line[i] <= '9'))
			i++;
		outfile = ft_strword(line + i);
		cmds->fd_in = open_file(outfile, 2);
	}
	return(remove_input_char(&line[0]));
}

void		get_redirect(char *line, int *count, t_cmds *cmds)
{
	char 	*outfile;
	int		i;

	if (line[0] == '|' && line[1] != '|')
	{
		if(cmds->fd_in)
			close(cmds->fd_in);
		cmds->fd_out = 1000;
	}
	else if (line[0] == '>' && line[1] == '>' && line[2] != '>')
	{
		i = 0;
		while (line [i] == '>' || line[i] == ' ')
			i++;
		outfile = ft_strword(line + i);
		cmds->fd_out = open_file(outfile, 0);
		while (line[i] >= 'a' && line[i] <= 'z' 
		|| line[i] >= 'A' && line[i] <= 'Z' 
		||  line[i] >= '0' && line[i] <= '9')
			i++;
		(*count) += i;
	}
	else if (line[0] == '>' && line[1] != '>' && line[2] != '>')
	{
		i = 0;
		while (line [i] == '>' || line[i] == ' ')
			i++;
		outfile = ft_strword(line + i);
		cmds->fd_out = open_file(outfile, 1);
		while (line[i] >= 'a' && line[i] <= 'z' 
		|| line[i] >= 'A' && line[i] <= 'Z' 
		||  line[i] >= '0' && line[i] <= '9')
			i++;
		(*count) += i;
	}
	else
		cmds->fd_out = 0;
}

int	*parser(char *line, t_data *data, char ***envp, S_SIG **act)
{
	t_cmds	*iter;
	int		j;

	data->cmds = malloc(sizeof (t_cmds));
	if (!data->cmds)
		cleanup(data, 1);
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
				break;
			iter->fd_in = 0;
			if(ft_strchr(line, '<'))
			{
				line = ft_strdup(treat_input_red(line + j, iter));
				if (!line)
					cleanup(data, 1);
			}
		}
		save_env_var(line + j, &j, data);
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
		get_redirect(line + j, &j, iter);
		while (line[j] == ' ')
			j++;
		if (iter->cmd[0] != '\0')
			if ((check_cmds(iter, data, *envp, act))) // 0 = executou
				exec_builtin(iter, data, envp, act);
		if (line[j] == '|' && line[j + 1] != '|')
		{
			iter->next = malloc(sizeof (t_cmds));
			if (!iter->next)
				cleanup(data, 1);
			iter = iter->next;
			init_cmds(iter);
			j++;
		}
	}
	return (0);
}
