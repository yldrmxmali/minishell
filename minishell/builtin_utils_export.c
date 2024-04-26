/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 20:51:41 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:16:37 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	export_key_control(char *text)
{
	int	i;

	if (text[0] == '=' || ft_isdigit(text[0]))
		return (0);
	i = 0;
	while (text[i] && text[i] != '=' && (ft_isalnum(text[i]) || text[i] == '_'))
		i++;
	if (text[i] == '=' || !text[i])
		return (1);
	return (0);
}

int	export_print(char *text, t_shell *my_shell)
{
	char	*key;
	char	*value;

	if (!export_key_control(text))
	{
		write(2, "minishell: export: `", 20);
		write(2, text, ft_strlen(text));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	if (ft_strchrindex_0(text, '=') != -1)
	{
		key = ft_substr(text, 0, ft_strchrindex(text, '='));
		value = ft_substr(text, ft_strchrindex(text, '=') + 1, (ft_strlen(text)
					- 1));
	}
	else
	{
		key = ft_substr(text, 0, ft_strlen(text));
		value = NULL;
	}
	edit_env(my_shell->env, key, value, my_shell);
	return (0);
}

void	declare_export(void *data, t_shell *my_shell)
{
	t_parse	*str;
	t_env	*new;
	int		i;

	i = 0;
	str = my_shell->parse;
	new = (t_env *)data;
	write(str->outfile, "declare -x ", 11);
	while (new->key[i])
		write(str->outfile, &new->key[i++], 1);
	if (new->value && *(new->value) && new->key[0] > 31)
		write(str->outfile, "=", 1);
	i = 0;
	while (new->value && new->value[i])
		write(str->outfile, &new->value[i++], 1);
	write(str->outfile, "\n", 1);
}

void	ft_newlstiter(t_list *lst, void (*f)(void *, t_shell *),
	t_shell *my_shell)
{
	if (!lst || !f)
		return ;
	while (lst != NULL)
	{
		f(lst->content, my_shell);
		lst = lst->next;
	}
}

int	exec_export(t_parse *data, t_shell *my_shell)
{
	int	i;

	i = 0;
	if (data->text == NULL)
		ft_newlstiter(my_shell->env, declare_export, my_shell);
	else
	{
		while (data->text[i])
		{
			if (export_print(data->text[i], my_shell))
			{
				my_shell->exec_status = 1;
				return (1);
			}
			i++;
		}
	}
	my_shell->exec_status = 0;
	return (0);
}
