/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 00:31:19 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:11:27 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	flag_free(t_parse *parse)
{
	free_text(&parse->text);
	parse->text = NULL;
}

void	free_parser_cmd(t_parse **parse)
{
	t_parse	*tmp;

	while (*parse)
	{
		tmp = *parse;
		*parse = (*parse)->next;
		if (tmp->cmd)
			free(tmp->cmd);
		if (tmp->text)
			free_text(&tmp->text);
		free(tmp);
	}
	*parse = NULL;
}

void	free_parser(t_shell *my_shell)
{
	t_parse	*parse;

	parse = my_shell->parse;
	if (parse)
		free_parser_cmd(&parse);
	else if (my_shell->parse)
		free(my_shell->parse);
}

void	free_text(char ***text)
{
	int	i;

	i = 0;
	if (*text)
	{
		if ((*text)[i])
		{
			while ((*text)[i])
				free((*text)[i++]);
		}
		free(*text);
		*text = NULL;
	}
}
