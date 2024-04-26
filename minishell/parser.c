/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 21:07:57 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:10:55 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

void	parse_text(t_parse *parse, char *str, int *j, int *flag)
{
	parse->text[*j] = ft_strdup(str);
	*j += 1;
	parse->text[*j] = NULL;
	*flag = 1;
}

int	parse_list_continue(t_parse *parse, t_list *lex, char *str, int a[3])
{
	while (lex != NULL)
	{
		str = lex->content;
		if (a[0]++ == 0 && ft_strcmp(str, "<<"))
		{
			parse->cmd = ft_strdup(str);
			if (parse->cmd == NULL)
				return (BAD_EXIT);
		}
		else if (str[0] == '|' || str[0] == '>' || str[0] == '<')
		{
			if (token_type(&parse, str), str[0] == '|')
				a[0] = 0;
			parse->next = parse_init(lex);
			a[1] = 0;
			remove_quote(parse);
			parse = parse->next;
		}
		else if (ft_strcmp(str, ""))
			parse_text(parse, str, &a[1], &a[2]);
		remove_quote(parse);
		lex = lex->next;
	}
	return (0);
}

int	parse_list(t_shell *my_shell, t_list *lex, char *str, int a[3])
{
	t_parse	*parse;

	parse = parse_init(my_shell->lexer_lst->lex);
	if (parse == NULL)
		return (BAD_EXIT);
	my_shell->parse = parse;
	if (parse_list_continue(parse, lex, str, a))
		return (BAD_EXIT);
	if (a[2] == 0)
		flag_free(parse);
	return (0);
}

int	check(t_list *lex_list, char *a, char *b)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = lex_list;
	while (tmp)
	{
		a = tmp->content;
		if (tmp->next)
			b = tmp->next->content;
		if (i == 0 && !b && ((a[0] == '<') || (a[0] == '>')))
			return (printf(SYNTX_ERROR), 1);
		if ((a[0] == '|' && !a[1]) && i == 0)
			return (printf(SYNTX_ERROR), 1);
		else if ((a[0] == '>' && !a[1]) && !b)
			return (printf(SYNTX_ERROR), 1);
		else if (is_valid_other(a, b))
			return (printf(SYNTX_ERROR), 1);
		tmp = tmp->next;
		i++;
	}
	return (0);
}

int	parser(t_shell *my_shell, char **env)
{
	int		control;
	char	*str;
	int		a[3];

	control = 0;
	str = NULL;
	ft_bzero(a, 12);
	control = check(my_shell->lexer_lst->lex, NULL, NULL);
	if (control)
		return (ft_lstclear(&(my_shell)->lexer_lst->lex, free),
			free(my_shell->cmd), 0);
	if (ft_strcmp(my_shell->cmd, ""))
	{
		if (parse_list(my_shell, my_shell->lexer_lst->lex, str, a))
			return (free_loop(1, my_shell), BAD_EXIT);
		ft_lstclear(&(my_shell)->lexer_lst->lex, free);
		if (create_files(my_shell) == 1)
			exec(env, my_shell);
	}
	free_loop(1, my_shell);
	free(my_shell->cmd);
	return (0);
}
