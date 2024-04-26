/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:41:53 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:24:15 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	run_command(char **env, t_parse *tmp, int *fd, t_shell *my_shell)
{
	int	control;

	control = is_builtin(tmp);
	if (control)
		execute_builtin_command(tmp, my_shell);
	else
		exec_others(tmp, env, fd, my_shell);
	free_loop(1, my_shell);
	free_init(&my_shell, 1);
	exit(255);
}

void	multi_command_(t_parse *parse, char **env, t_shell *my_shell, int *fd)
{
	t_parse	*nparse;

	while (parse)
	{
		if (parse->next)
			pipe(fd);
		nparse = _next_command(&parse);
		parse->pid = fork();
		if (parse->pid == 0)
		{
			create_dup_one(parse, fd);
			run_command(env, parse, fd, my_shell);
		}
		if (nparse)
			create_dup_two(nparse, fd);
		parse = nparse;
	}
}

void	only_single_command(t_shell *my_shell, t_parse *parse, char **env)
{
	int	control;

	control = 0;
	if (parse->type != HEREDOC)
		control = is_builtin(parse);
	if (control)
		execute_builtin_command(my_shell->parse, my_shell);
	else
		exec_others(my_shell->parse, env, NULL, my_shell);
}

void	only_multi_command( t_shell *my_shell, char **env,
	t_parse *parse, int x)
{
	int	*fd;

	(void)x;
	my_shell->parse->std_in = dup(0);
	fd = (int *)malloc(sizeof(int) * 2);
	if (!fd)
		return ;
	multi_command_(parse, env, my_shell, fd);
	dup2(my_shell->parse->std_in, 0);
	clear_pipe(fd);
	wait_all(my_shell);
}

void	exec(char **env, t_shell *my_shell)
{
	int	x;

	if (g_heredoc_sig != 0)
		loop_heredoc(my_shell);
	if (my_shell->parse->cmd && !ft_strcmp(my_shell->parse->cmd, "exit")
		&& g_heredoc_sig != -10 && _next_command(&my_shell->parse) == NULL)
	{
		builtin_exit(&my_shell);
		return ;
	}
	x = single_or_multi_command(my_shell);
	if (!x && g_heredoc_sig != -10)
		only_single_command(my_shell, my_shell->parse, env);
	else if (g_heredoc_sig != -10)
		only_multi_command(my_shell, env, my_shell->parse, 0);
	g_heredoc_sig = 0;
}
