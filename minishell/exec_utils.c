/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 00:08:48 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:23:15 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

void	wait_all(t_shell *my_shell)
{
	t_parse	*data;

	data = my_shell->parse;
	while (data)
	{
		if (!data->next)
			waitpid(data->pid, &my_shell->exec_status, 0);
		else
			waitpid(data->pid, 0, 0);
		data = data->next;
	}
	my_shell->exec_status = WEXITSTATUS(my_shell->exec_status);
}

int	single_or_multi_command(t_shell *my_shell)
{
	t_parse	*parse;
	int		i;

	i = 0;
	parse = my_shell->parse;
	while (parse)
	{
		if (parse->type == 2)
			i++;
		parse = parse->next;
	}
	return (i);
}

t_parse	*_next_command(t_parse **parse)
{
	if ((*parse)->infile > STDERR || (*parse)->outfile > STDERR
		|| (*parse)->type == HEREDOC)
	{
		return ((*parse)->next->next);
	}
	else
	{
		return ((*parse)->next);
	}
}
