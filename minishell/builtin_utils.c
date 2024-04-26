/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:41:41 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:18:35 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

void	print_list(void *data, t_shell *my_shell)
{
	t_parse	*str;
	t_env	*new;
	int		i;

	i = 0;
	str = my_shell->parse;
	new = (t_env *)data;
	if (!new->value)
		return ;
	while (new->key[i])
		write(str->outfile, &new->key[i++], 1);
	write(str->outfile, "=", 1);
	i = 0;
	while (new->value && new->value[i])
		write(str->outfile, &new->value[i++], 1);
	write(str->outfile, "\n", 1);
}

void	exec_env(t_shell *my_shell)
{
	ft_newlstiter(my_shell->env, print_list, my_shell);
	my_shell->exec_status = 0;
}

void	run_echo(t_parse *data, int *i)
{
	int	x;

	x = *i;
	while (data->text[x])
	{
		write(data->outfile, data->text[x], ft_strlen(data->text[x]));
		if (data->text[x + 1] != NULL)
			write(data->outfile, " ", 1);
		x++;
		*i += 1;
	}
}

static int	all_n_chars(const char *str)
{
	while (*str != '\0')
	{
		if (*str != 'n')
			return (0);
		str++;
	}
	return (1);
}

void	exec_echo(t_parse *data, t_shell *my_shell)
{
	int	i;

	i = 0;
	if (!data->text)
		write(data->outfile, "\n", 1);
	else if (!data->text || data->text[0] == NULL || data->text[0][0] == '\0')
		write(data->outfile, "", 1);
	else
	{
		if (data->text[0][0] == '-' && data->text[0][1] == 'n'
			&& all_n_chars(data->text[0] + 2))
		{
			i = 1;
			if (data->text[1])
				run_echo(data, &i);
		}
		else
		{
			run_echo(data, &i);
			if (!data->text[i])
				write(data->outfile, "\n", 1);
		}
	}
	my_shell->exec_status = 0;
}
