/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 19:05:25 by felipe            #+#    #+#             */
/*   Updated: 2021/12/20 12:10:56 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset_env(char ***envp, t_args *iter)
{
	int	i;
	int	j;

	j = 0;
	i = -1;
	while ((*envp)[++i] != 0)
	{
		if (!ft_strncmp(iter->arg, (*envp)[i], ft_strlen(iter->arg)) \
		&& (*envp)[i][ft_strlen(iter->arg)] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = 0;
			j++;
		}
		(*envp)[i] = (*envp)[j];
		j++;
	}
}

void	unset_vars(t_args *iter, t_vars **variables)
{
	t_vars	*i_vars;
	t_vars	*prev;

	i_vars = *variables;
	while (i_vars)
	{
		if (!ft_strncmp(iter->arg, i_vars->var, ft_strlen(iter->arg)))
		{
			if (i_vars == *variables)
			{
				prev = *variables;
				*variables = (*variables)->next;
				free(prev);
				break ;
			}
			prev->next = i_vars->next;
			free(i_vars->var);
			free(i_vars->value);
			free(i_vars);
			break ;
		}
		prev = i_vars;
		i_vars = i_vars->next;
	}
}

int	builtin_unset(t_cmds *cmds, t_vars **variables, char ***envp)
{
	t_args	*iter;

	iter = cmds->args;
	while (iter)
	{
		unset_env(envp, iter);
		unset_vars(iter, variables);
		iter = iter->next;
	}
	return (0);
}
