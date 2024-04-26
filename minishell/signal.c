/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 17:21:57 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:12:00 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <unistd.h>

int	expand_question_mark(t_shell *shell, t_list *lex, char **temp,
		char *before)
{
	char	*after;
	char	*back;
	char	*new_value;

	after = ft_strdup(*temp + 2);
	if (!after)
		return (BAD_EXIT);
	new_value = ft_itoa(shell->exec_status);
	back = ft_strjoin(new_value, after);
	if (!back)
		return (free(after), free(new_value), BAD_EXIT);
	free(new_value);
	free(after);
	free(lex->content);
	lex->content = ft_strjoin(before, back);
	if (!lex->content)
		return (free(back), BAD_EXIT);
	free(back);
	*temp = ft_strchr(lex->content + ft_strlen(before), '$');
	return (0);
}

int	expand_dollar_variable_continue(t_shell *shell, t_list *lex, char **temp,
		char *before)
{
	char	*new_value;

	new_value = env_get(shell->env, *temp + 1);
	free(lex->content);
	lex->content = ft_strjoin(before, new_value);
	if ((char *)lex->content == NULL)
	{
		lex->content = ft_strdup(before);
		if (!lex->content)
			return (free(new_value), BAD_EXIT);
	}
	free(new_value);
	*temp = ft_strchr(lex->content + ft_strlen(before), '$');
	return (0);
}

int	create_files(t_shell *my_shell)
{
	t_parse	*current_parse;
	t_parse	*firts_parse;
	int		i;

	i = 1;
	current_parse = my_shell->parse;
	firts_parse = my_shell->parse;
	while (current_parse)
	{
		if (current_parse->type == 3 || current_parse->type == 4)
			create_out_file(current_parse, firts_parse, my_shell);
		else if (current_parse->type == 5)
			i = create_in_files_meta(current_parse, my_shell);
		current_parse = current_parse->next;
	}
	return (i);
}

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	signal_control(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
