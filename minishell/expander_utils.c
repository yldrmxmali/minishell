/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:34:45 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:15:20 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_ultimatestrcmp(char *struct_env, char *our_env, int i, int *flag)
{
	int	j;

	j = 0;
	while (our_env[i++] == struct_env[j++])
	{
		if (!struct_env[j])
		{
			if (!ft_isalnum(our_env[i]))
				return (i);
			while (ft_isalnum(our_env[i]))
				i++;
			if (our_env[i])
			{
				*flag = 1;
				return (i);
			}
			return (0);
		}
	}
	return (0);
}

static void	if_there_is_no_env(char *key, char **ret)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (key[i] && ft_isalnum(key[i]))
		i++;
	j = i;
	while (key[j])
		j++;
	*ret = ft_substr(key, i, j - i);
}

char	*env_get(t_list *env, char *our_env)
{
	t_env	*struct_env;
	char	*ret;
	int		flag;

	flag = 0;
	ret = (void *)0;
	while (env)
	{
		struct_env = env->content;
		if (ft_ultimatestrcmp(struct_env->key, our_env, 0, &flag))
		{
			if (flag == 1)
				ret = ft_strdup(our_env + ft_ultimatestrcmp(struct_env->key,
							our_env, 0, &flag));
			else
				ret = ft_strjoin(struct_env->value, our_env
						+ ft_ultimatestrcmp(struct_env->key,
							our_env, 0, &flag));
			return (ret);
		}
		env = env->next;
	}
	if_there_is_no_env(our_env, &ret);
	return (ret);
}

int	is_count_odd(char *before, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (before[i])
	{
		if (before[i] == c)
			count++;
		i++;
	}
	if (count == 0)
		return (-2);
	i = 0;
	if (count % 2 == 1)
	{
		while (before[i])
		{
			if (before[i] == c)
				return (i);
			i++;
		}
	}
	return (-1);
}

int	check_quote(char *before, char *after)
{
	int	single_quotes_before;
	int	single_quotes_after;
	int	double_quotes_before;
	int	double_quotes_after;

	single_quotes_before = is_count_odd(before, '\'');
	single_quotes_after = is_count_odd(after, '\'');
	double_quotes_before = is_count_odd(before, '\"');
	double_quotes_after = is_count_odd(after, '\"');
	if (double_quotes_after > -1 && after[1] == '\"')
		return (0);
	else if ((single_quotes_before > -1 && single_quotes_after < 0))
		return (1);
	else if ((single_quotes_before > -1 && single_quotes_after > -1)
		&& (double_quotes_before > -1 && double_quotes_after > -1))
		return (single_quotes_before > double_quotes_before);
	else if ((single_quotes_before > -1 && single_quotes_after > -1)
		&& (double_quotes_before < 0 && double_quotes_after < 0))
		return (0);
	else if ((single_quotes_before < 0 && single_quotes_after < 0)
		&& (double_quotes_before > -1 && double_quotes_after > -1))
		return (1);
	return (1);
}
