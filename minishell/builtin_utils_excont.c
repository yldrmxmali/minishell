/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_excont.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 03:49:20 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:16:53 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	edit_env_(t_list *node, char *key, char *value, t_shell *my_shell)
{
	t_env	*new_env;

	if (node == NULL)
	{
		new_env = malloc(sizeof(t_env));
		if (!new_env)
			return ;
		new_env->key = key;
		new_env->value = value;
		ft_lstadd_back(&(my_shell->env), ft_lstnew(new_env));
	}
}

void	edit_env(t_list *node, char *key, char *value, t_shell *my_shell)
{
	t_env	*env;

	while (node != NULL)
	{
		env = (t_env *)node->content;
		if (ft_strcmp(env->key, key) == 0)
		{
			if (env->value)
				free(env->value);
			env->value = ft_strdup(value);
			if (value)
				free(value);
			free(key);
			return ;
		}
		node = node->next;
	}
	edit_env_(node, key, value, my_shell);
}

void	non_old(t_shell *my_shell, char *str, char *targetkey)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->key = ft_strdup(targetkey);
	env->value = ft_strdup(str);
	ft_lstadd_back(&my_shell->env, ft_lstnew(env));
}
