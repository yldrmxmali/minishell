/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 21:22:00 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:25:14 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	*search_command(char *cmd, char **value)
{
	char	*data;
	size_t	length;
	char	*substring;
	char	*tmp;

	substring = NULL;
	data = NULL;
	tmp = *value;
	while (*tmp && *tmp != ':')
		tmp++;
	length = tmp - *value;
	if (!length)
		return (NULL);
	substring = ft_substr(*value, 0, length);
	data = ft_strjoin(substring, "/");
	free(substring);
	substring = ft_strjoin(data, cmd);
	free(data);
	if (*tmp)
		*value = tmp + 1;
	return (substring);
}

char	*_search_path(t_shell *my_shell)
{
	t_env	*env;
	t_list	*tmp;
	char	*value;

	value = NULL;
	tmp = my_shell->env;
	while (tmp)
	{
		env = tmp->content;
		if (ft_strcmp(env->key, "PATH") == 0)
		{
			value = env->value;
			return (value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	search_path(t_parse *data, int i, t_shell *my_shell)
{
	char	*value;
	char	*str;
	int		x;

	x = -1;
	str = NULL;
	value = _search_path(my_shell);
	while (x != 0 && value && ++i < 10)
	{
		if (str)
			free(str);
		str = search_command(data->cmd, &value);
		if (str)
			x = access(str, F_OK);
		if (str && x == 0)
		{
			if (data->cmd)
				free(data->cmd);
			data->cmd = ft_strdup(str);
		}
	}
	if (str)
		free(str);
}

void	run_execve(t_parse *parse, char **env, int *fd, t_shell *my_shell)
{
	char	**full_cmd;

	search_path(parse, -1, my_shell);
	parse->pid = fork();
	full_cmd = NULL;
	if (!parse->pid)
	{
		full_cmd = get_args(parse);
		create_dup(my_shell, parse);
		if (!full_cmd || !parse->cmd || (execve(parse->cmd, full_cmd, env)
				== -1))
		{
			if (parse->cmd && ft_strcmp(parse->cmd, "<<"))
				printf("minishell: %s: command not found\n", parse->cmd);
			free_loop(1, my_shell);
			free_text(&full_cmd);
			free_init(&my_shell, 1);
			exit(127);
		}
	}
	if (fd && parse->pid)
		clear_pipe(fd);
	waitpid(parse->pid, &my_shell->exec_status, 0);
	my_shell->exec_status = WEXITSTATUS(my_shell->exec_status);
}

void	exec_others(t_parse *parse, char **env, int *fd, t_shell *my_shell)
{
	run_execve(parse, env, fd, my_shell);
}
