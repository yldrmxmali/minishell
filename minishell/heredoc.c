/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:43:57 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 03:54:20 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void	get_var(int *fd, t_shell *my_shell)
{
	char	*a;

	a = malloc(sizeof(char));
	if (!a)
	{
		my_shell->heredoc = NULL;
		return ;
	}
	while (read(fd[0], a, 1))
	{
		my_shell->heredoc = add_char(my_shell->heredoc, *a);
		free(a);
		a = malloc(sizeof(char));
		if (!a)
			return ;
	}
	if (a)
		free(a);
	close(fd[0]);
}

void	put_buffer(char *buffer, int *fd)
{
	write(fd[1], buffer, ft_strlen(buffer));
	write(fd[1], "\n", 1);
	free(buffer);
}

void	get_input(t_parse *parse, int *fd, t_shell *my_shell)
{
	char	*delimiter;
	char	*buffer;
	int		flag;

	flag = 0;
	delimiter = parse->next->text[0];
	delimiter = heredoc_control(my_shell, delimiter, &flag);
	while (1 && g_heredoc_sig != -3)
	{
		buffer = readline("> ");
		if (buffer == NULL)
			break ;
		if (ft_strcmp(buffer, delimiter) == 0)
		{
			if (buffer)
				free(buffer);
			if (flag)
				free(delimiter);
			break ;
		}
		if (buffer)
			put_buffer(buffer, fd);
	}
}

void	heredoc(t_parse *parse, t_shell *my_shell)
{
	int	fd[2];
	int	id;

	pipe(fd);
	signal(SIGINT, set_heredoc);
	id = fork();
	if (id == 0)
	{
		signal(SIGINT, killer);
		close(fd[0]);
		get_input(parse, fd, my_shell);
		close(fd[1]);
		free_loop(1, my_shell);
		free_init(&my_shell, 1);
		exit(0);
	}
	waitpid(-1, NULL, 0);
	signal(SIGINT, sigint_handler);
	close(fd[1]);
	get_var(fd, my_shell);
}

void	loop_heredoc(t_shell *my_shell)
{
	t_parse	*parse;

	parse = my_shell->parse;
	while (parse)
	{
		if (parse->type == HEREDOC)
		{
			heredoc(parse, my_shell);
		}
		parse = parse->next;
	}
}
