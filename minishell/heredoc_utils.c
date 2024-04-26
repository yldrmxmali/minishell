/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 22:47:03 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:26:44 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <sys/ioctl.h>

char	*heredoc_control(t_shell *my_shell, char *delimiter, int *flag)
{
	t_list	*tmp;
	t_env	*env;
	char	*tmp_delimiter;

	tmp = my_shell->env;
	while (tmp)
	{
		env = (t_env *)tmp->content;
		if (ft_strcmp(env->value, delimiter) == 0)
		{
			tmp_delimiter = ft_strjoin("$", env->key);
			*flag = 1;
			return (tmp_delimiter);
		}
		tmp = tmp->next;
	}
	return (delimiter);
}

char	*add_char(char *str, char a)
{
	char	*tmp;
	int		i;
	int		len;

	if (str == NULL)
		len = 0;
	else
		len = ft_strlen(str);
	tmp = ft_calloc(sizeof(char) * (len + 2), 1);
	if (!tmp)
		return (NULL);
	i = 0;
	if (!a)
	{
		free(tmp);
		return (NULL);
	}
	while (i < len)
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = a;
	free(str);
	return (tmp);
}

void	killer(int sig)
{
	(void)sig;
	g_heredoc_sig = -3;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	set_heredoc(int sig)
{
	(void)sig;
	g_heredoc_sig = -10;
}
