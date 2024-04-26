/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 17:59:06 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:10:40 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int		g_heredoc_sig;

void	print_readline(t_shell *my_shell)
{
	my_shell->cmd = readline("minishell$ ");
	if (my_shell->cmd && !is_space(my_shell->cmd))
	{
		free(my_shell->cmd);
		print_readline(my_shell);
	}
	else if (!my_shell->cmd)
	{
		free_loop(0, my_shell);
		clear_history();
		free_init(&my_shell, 1);
		exit(1);
	}
	add_history(my_shell->cmd);
}

static int	init(t_shell **my_shell)
{
	if (!(*my_shell)->env)
		return (BAD_EXIT);
	(*my_shell)->env->content = NULL;
	(*my_shell)->env->next = NULL;
	(*my_shell)->lexer_lst->lex = NULL;
	g_heredoc_sig = 0;
	return (0);
}

static int	null_init(t_shell **my_shell)
{
	*my_shell = malloc(sizeof(t_shell));
	if (!(*my_shell))
		return (BAD_EXIT);
	(*my_shell)->lexer_lst = malloc(sizeof(t_lexer));
	if (!(*my_shell)->lexer_lst)
		return (free(*my_shell), BAD_EXIT);
	(*my_shell)->env = malloc(sizeof(t_env));
	(*my_shell)->lexer_lst->lex = NULL;
	(*my_shell)->parse = NULL;
	(*my_shell)->cmd = NULL;
	(*my_shell)->heredoc = NULL;
	(*my_shell)->exec_status = 0;
	g_heredoc_sig = 0;
	return (0);
}

int	contunie_mini(t_shell *my_shell, char **env)
{
	t_list	*lex;

	lex = NULL;
	while (1)
	{
		print_readline(my_shell);
		if (quate_check(my_shell->cmd, '\'', '\"') == 0)
		{
			if (lexer(my_shell) == BAD_EXIT)
				return (free_init(&my_shell, 1), printf(ALLOC_ERROR), BAD_EXIT);
			if (expander(my_shell) == BAD_EXIT)
				return (free_init(&my_shell, 1), printf(ALLOC_ERROR), BAD_EXIT);
			if (parser(my_shell, env) == BAD_EXIT)
				return (free_init(&my_shell, 1), printf(ALLOC_ERROR), BAD_EXIT);
		}
		else
		{
			free(my_shell->cmd);
			continue ;
		}
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	*my_shell;

	(void)av;
	if (ac != 1)
		exit (printf(W_ARGS));
	if (null_init(&my_shell) == BAD_EXIT)
		return (printf(ALLOC_ERROR), BAD_EXIT);
	if (init(&my_shell) == BAD_EXIT)
		return (free_init(&my_shell, 1), printf(ALLOC_ERROR), BAD_EXIT);
	if (get_env(&my_shell, env) == BAD_EXIT)
		return (free_init(&my_shell, 1), printf(ALLOC_ERROR), BAD_EXIT);
	signal_control();
	if (contunie_mini(my_shell, env) == BAD_EXIT)
		return (BAD_EXIT);
}
