/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 17:20:08 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:14:06 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <fcntl.h>

void	clear_pipe(int *fd)
{
	int	index;

	if (fd)
	{
		index = -1;
		while (++index < 2)
		{
			if (fd[index])
				close(fd[index]);
		}
		if (fd)
			free(fd);
	}
}

void	del_env(void *content)
{
	t_env	*tmp;

	tmp = content;
	if (!tmp)
		return ;
	if (tmp->key)
		free(tmp->key);
	if (tmp->value)
		free(tmp->value);
	free(tmp);
}

void	free_loop(int control, t_shell *m_shell)
{
	if (control)
	{
		if (m_shell->parse)
			free_parser(m_shell);
	}
	if (m_shell->heredoc)
	{
		free(m_shell->heredoc);
		m_shell->heredoc = NULL;
	}
}

void	free_init(t_shell **my_shell, int control)
{
	rl_clear_history();
	if (control == 1)
	{
		if ((*my_shell)->lexer_lst && control == 1)
		{
			ft_lstclear(&(*my_shell)->lexer_lst->lex, free);
			free((*my_shell)->lexer_lst->lex);
		}
		if ((*my_shell)->lexer_lst)
		{
			free((*my_shell)->lexer_lst);
		}
		if ((*my_shell)->env)
			ft_lstclear(&(*my_shell)->env, del_env);
		if ((*my_shell)->env)
			free((*my_shell)->env);
	}
	if ((*my_shell)->cmd)
		free((*my_shell)->cmd);
	if ((*my_shell))
		free(*my_shell);
}
