/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 10:12:42 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:14:28 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

int	expand_dollar_variable(t_shell *shell, t_list *lex, char **temp,
		char *before)
{
	char	*new_value;
	int		flag;

	flag = 0;
	if (ft_isdigit((*temp)[1]))
	{
		new_value = ft_strdup(*temp + 2);
		if (!new_value)
			return (BAD_EXIT);
		free(lex->content);
		lex->content = ft_strjoin(before, new_value);
		if ((char *)lex->content == NULL)
			return (free(new_value), BAD_EXIT);
		free(new_value);
		*temp = ft_strchr(lex->content + ft_strlen(before), '$');
	}
	else if (!ft_isdigit((*temp)[1]))
	{
		flag = expand_dollar_variable_continue(shell, lex, temp, before);
		if (flag == BAD_EXIT)
			return (BAD_EXIT);
	}
	return (0);
}

int	dollar_parse(t_shell *shell, t_list *lex, char **temp, char *before)
{
	int	flag;

	flag = 0;
	if ((*temp)[1] == '?')
	{
		flag = expand_question_mark(shell, lex, temp, before);
		if (flag == BAD_EXIT)
			return (BAD_EXIT);
	}
	else if ((*temp)[1] != '$' && ft_strcmp((*temp), "$") != 0)
	{
		if (expand_dollar_variable(shell, lex, temp, before))
			return (BAD_EXIT);
	}
	else
		*temp = ft_strchr(*temp + 1, '$');
	return (0);
}

static int	expander_tilde(t_shell *my_shell, t_list *lex)
{
	char	*tmp;
	char	*home;

	home = env_get(my_shell->env, "HOME");
	if (!home)
		return (BAD_EXIT);
	if (((char *)lex->content)[0] == '~' && ((char *)lex->content)[1] == '/')
	{
		tmp = ft_strdup(lex->content);
		free(lex->content);
		lex->content = ft_strjoin(home, tmp + 1);
		free(tmp);
		free(home);
	}
	else if (((char *)lex->content)[0] == '~'
		&& ((char *)lex->content)[1] == '\0')
	{
		free(lex->content);
		lex->content = home;
	}
	else
		free(home);
	return (0);
}

int	exparder_continue(t_shell *my_shell, t_list *lex)
{
	char	*temp;
	char	*before;

	temp = ft_strchr(lex->content, '$');
	while (temp)
	{
		before = ft_substr(lex->content, 0, temp - (char *)lex->content);
		if (!before)
			return (BAD_EXIT);
		if (check_quote(before, temp))
		{
			if (dollar_parse(my_shell, lex, &temp, before))
				return (free(before), BAD_EXIT);
		}
		else
			temp = ft_strchr(temp + 1, '$');
		free(before);
	}
	return (0);
}

int	expander(t_shell *my_shell)
{
	t_list	*lex;

	lex = my_shell->lexer_lst->lex;
	while (lex)
	{
		if (((char *)lex->content)[0] == '~')
			if (expander_tilde(my_shell, lex) == 1)
				return (BAD_EXIT);
		if (exparder_continue(my_shell, lex) == BAD_EXIT)
			return (BAD_EXIT);
		lex = lex->next;
	}
	return (0);
}
