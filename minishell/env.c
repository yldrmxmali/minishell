/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 11:24:26 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:10:30 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	ft_str(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	get_env(t_shell **my_shell, char **env)
{
	t_env	*a;
	char	**str;

	free((*my_shell)->env);
	(*my_shell)->env = NULL;
	while (*env)
	{
		a = malloc(sizeof(t_env));
		str = ft_split(*env, '=');
		if (!str)
			return (free(a), 1);
		if (str && str[0] && str[1])
		{
			a->key = ft_strdup(str[0]);
			a->value = ft_strdup(str[1]);
			if (a->key && a->value)
				ft_lstadd_back(&(*my_shell)->env, ft_lstnew(a));
			else
				return (ft_str(str), free(a->key), free(a->value), free(a), 1);
		}
		ft_str(str);
		env++;
	}
	return (0);
}
