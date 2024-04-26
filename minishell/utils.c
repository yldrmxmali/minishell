/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:23:27 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:12:13 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

int	quate_check(char *cmd, char c, char d)
{
	int	i;
	int	quate;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == c || cmd[i] == d)
		{
			quate = cmd[i];
			i++;
			while (cmd[i] && cmd[i] != quate)
				i++;
			if (cmd[i] == '\0')
				return (printf(QUATE_ERROR), BAD_EXIT);
		}
		i++;
	}
	return (0);
}

void	dollar_countine(char *cmd, size_t *i, size_t *j)
{
	while ((cmd[*i + *j] && ft_isalnum(cmd[*i + *j]))
		|| (cmd[*i + *j] == '?'))
		(*j)++;
}

char	*ft_strjoin_char(char *s1, char s2)
{
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1)
		return (NULL);
	ret = (char *)malloc(sizeof(char) * (ft_strlen(s1) + 2));
	if (!ret)
		return (NULL);
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	ret[i++] = s2;
	ret[i] = '\0';
	free(s1);
	return (ret);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	is_space(char *cmd)
{
	int	i;

	i = 0;
	while (*cmd && *cmd == ' ')
		cmd++;
	if (!*cmd)
		return (0);
	return (1);
}
