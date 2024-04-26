/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 20:37:37 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:10:21 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	ignore_quote(char const *cmd, int i)
{
	if (cmd[i] == '\"')
	{
		i++;
		while (cmd[i] != '\"')
			i++;
	}
	else if (cmd[i] == '\'')
	{
		i++;
		while (cmd[i] != '\'')
			i++;
	}
	return (i);
}

static int	len_for_trimmed(char *cmd)
{
	int	i;

	i = 0;
	if (!cmd || !*cmd)
		return (0);
	if (cmd[i] == '<' && cmd[i + 1] == '<')
		return (2);
	if (cmd[i] == '>' && cmd[i + 1] == '>')
		return (2);
	if (cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|')
		return (1);
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '>' && cmd[i] != '<'
		&& cmd[i] != '|')
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
		{
			i = ignore_quote(cmd, i) + 1;
		}
		else
			i++;
	}
	return (i);
}

int	lexer_start(t_shell *my_shell, char *cmd, t_lexer **lexer_lst)
{
	int		len_cmd;
	char	*tmp;
	char	*tmp_lst;

	if (!cmd || !*cmd)
		return (BAD_EXIT);
	cmd = ft_strtrim(cmd, " ");
	if (!cmd)
		return (BAD_EXIT);
	len_cmd = len_for_trimmed(cmd);
	tmp = ft_substr(cmd, 0, len_cmd);
	if (!tmp)
		return (free(cmd), BAD_EXIT);
	tmp_lst = ft_strtrim(tmp, " ");
	if (!tmp_lst)
		return (free(tmp), free(cmd), printf(ALLOC_ERROR), BAD_EXIT);
	free(tmp);
	ft_lstadd_back(&(*lexer_lst)->lex, ft_lstnew(tmp_lst));
	lexer_start(my_shell, cmd + len_cmd, lexer_lst);
	free(cmd);
	return (0);
}

int	lexer(t_shell *my_shell)
{
	my_shell->lexer_lst->lex = NULL;
	if (lexer_start(my_shell, my_shell->cmd, &my_shell->lexer_lst) == BAD_EXIT)
		return (BAD_EXIT);
	return (0);
}
