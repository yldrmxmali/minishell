/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_exec.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:45:00 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:16:17 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void	change_pwd(t_parse *data, t_shell *my_shell)
{
	char	*targetkey;
	t_list	*node;
	t_env	*env;

	targetkey = "PWD";
	node = my_shell->env;
	while (node)
	{
		env = (t_env *)node->content;
		if (!ft_strcmp(env->key, targetkey))
		{
			if (env->value)
				free(env->value);
			if (data->text && !ft_strcmp(data->text[0], "~"))
				env->value = get_home(my_shell);
			else
				env->value = getcwd(NULL, 0);
			return ;
		}
		node = node->next;
	}
}

void	change_old(char *str, t_shell *my_shell)
{
	t_list	*node;
	t_env	*env;
	char	*targetkey;
	int		flag;

	targetkey = "OLDPWD";
	node = my_shell->env;
	while (node)
	{
		env = (t_env *)node->content;
		if (!ft_strcmp(env->key, targetkey))
		{
			free(env->value);
			env->value = ft_strdup(str);
			return ;
		}
		node = node->next;
		if (!node)
			flag = 1;
	}
	if (flag)
		non_old(my_shell, str, targetkey);
}

void	exec_pwd(t_parse *data, t_shell *my_shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		write(data->outfile, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		my_shell->exec_status = 0;
		free(cwd);
	}
	else
	{
		perror("getcwd");
		my_shell->exec_status = 1;
	}
}

void	exec_cd1(char *str, t_parse *data, t_shell *my_shell)
{
	change_old(str, my_shell);
	if (get_home(my_shell))
		chdir(get_home(my_shell));
	change_pwd(data, my_shell);
	free(str);
	my_shell->exec_status = 0;
}

void	exec_cd(t_parse *data, t_shell *my_shell)
{
	char	*str;

	str = getcwd(NULL, 0);
	if (data->text && data->text[0])
	{
		if (chdir(data->text[0]) == 0)
		{
			if (str)
				change_old(str, my_shell);
			change_pwd(data, my_shell);
			my_shell->exec_status = 0;
			if (str)
				free(str);
		}
		else
		{
			if (str)
				free(str);
			my_shell->exec_status = 1;
			perror("cd");
		}
	}
	else
		exec_cd1(str, data, my_shell);
}
