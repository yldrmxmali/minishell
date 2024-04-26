/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 21:25:21 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:25:42 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	_create_dup(t_shell *my_shell)
{
	int	new_fd[2];

	pipe(new_fd);
	if (!!my_shell->heredoc)
		write(new_fd[1], my_shell->heredoc, ft_strlen(my_shell->heredoc));
	dup2(new_fd[0], 0);
	close(new_fd[1]);
	close(new_fd[0]);
}

void	create_dup(t_shell *my_shell, t_parse *parse)
{
	if (parse->type == HEREDOC)
		_create_dup(my_shell);
	if (parse->infile > STDERR)
		dup2(parse->infile, 0);
	if (parse->outfile > STDERR)
		dup2(parse->outfile, 1);
}

char	**get_args(t_parse *parse)
{
	char	**exec;
	int		i;
	int		j;

	j = 0;
	if (parse->text)
		while (parse->text[j])
			j++;
	exec = malloc(sizeof(char *) * (j + 2));
	if (!exec)
		return (NULL);
	i = 0;
	if (!parse->cmd)
		return (free(exec), NULL);
	exec[0] = ft_strdup(parse->cmd);
	j = 1;
	if (parse->text)
	{
		while (parse->text[i])
		{
			exec[j] = ft_strdup(parse->text[i++]);
			j++;
		}
	}
	return (exec[j] = NULL, exec);
}
