/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 00:20:42 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:11:14 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	token_type(t_parse **parse, const char *str)
{
	if (str[0] == '|')
		(*parse)->type = PIPE;
	else if (str[0] == '>' && str[1] == '>')
		(*parse)->type = GREATER;
	else if (str[0] == '<' && str[1] == '<')
	{
		(*parse)->type = HEREDOC;
		g_heredoc_sig = 1;
	}
	else if (str[0] == '>')
		(*parse)->type = GREAT;
	else if (str[0] == '<')
		(*parse)->type = LESS;
}

void	remove_quotes_in_str(char *str)
{
	int		i;
	int		j;
	int		in_quotes;
	char	quote_char;

	i = 0;
	j = 0;
	in_quotes = 0;
	quote_char = '\0';
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && (!in_quotes
				|| quote_char == str[i]))
		{
			in_quotes = !in_quotes;
			if (in_quotes)
				quote_char = str[i];
			else
				quote_char = '\0';
		}
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

void	remove_quote(t_parse *parse)
{
	char	**text_ptr;

	if (parse != NULL)
	{
		if (parse->cmd != NULL)
			remove_quotes_in_str(parse->cmd);
		if (parse->text != NULL)
		{
			text_ptr = parse->text;
			while (*text_ptr != NULL)
			{
				remove_quotes_in_str(*text_ptr);
				text_ptr++;
			}
		}
		remove_quote(parse->next);
	}
}

int	is_valid_other(char *a, char *b)
{
	if ((a[0] == '<' && !a[1]) && !b)
		return (1);
	else if ((a[0] == '>' && !a[1]) && (!ft_strcmp(a, b)))
		return (1);
	else if (((a[0] == '<' && !a[1])) && (!ft_strcmp(a, b)))
		return (1);
	else if (((a[0] == '<' && a[1] == '<')) && (!ft_strcmp(a, b)))
		return (1);
	else if (((a[0] == '>' && a[1] == '>')) && (!ft_strcmp(a, b)))
		return (1);
	else if (((a[0] == '<' && a[1] != '<')) && (b[0] == '<' || b[0] == '>'
			|| b[0] == '|'))
		return (1);
	else if ((a[0] == '>' && a[1] != '>') && (b[0] == '<' || b[0] == '>'
			|| b[0] == '|'))
		return (1);
	else if (((a[0] == '<' && a[1] == '<')) && (b[0] == '>' || b[0] == '|'))
		return (1);
	else if ((a[0] == '>' && a[1] == '>') && (b[0] == '<' || b[0] == '>'
			|| b[0] == '|'))
		return (1);
	else if ((a[0] == '|' && !a[1] && b[0] == '|' && !b[1]))
		return (1);
	return (0);
}

t_parse	*parse_init(t_list *lex)
{
	t_parse	*parse;

	parse = malloc(sizeof(t_parse));
	if (!parse)
		return (NULL);
	(parse)->next = NULL;
	(parse)->cmd = NULL;
	(parse)->text = ft_calloc(sizeof(char *), (sizeof(ft_lstsize(lex)) + 1));
	if (!(parse)->text)
		return (NULL);
	(parse)->type = 0;
	(parse)->infile = STDINN;
	(parse)->outfile = STDOUT;
	(parse)->fd = 1;
	(parse)->pid = 0;
	(parse)->control = 0;
	return (parse);
}
