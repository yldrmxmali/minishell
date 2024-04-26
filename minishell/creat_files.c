/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:53:59 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:20:48 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

void	other_out_files_meta(t_parse *parse, t_shell *my_shell)
{
	char	*pwd1;
	t_parse	*nparse;
	char	*home;

	home = env_get(my_shell->env, "HOME");
	pwd1 = NULL;
	nparse = parse->next;
	if (!ft_strnstr(nparse->text[0], home, ft_strlen(home)))
		handle_relative_path(&pwd1, parse);
	else
		pwd1 = ft_strdup(nparse->text[0]);
	if (nparse->type == 4)
		parse->fd = open(pwd1, O_CREAT | O_RDWR | O_APPEND, 0777);
	else if (nparse->type == 3)
		parse->fd = open(pwd1, O_CREAT | O_RDWR | O_TRUNC, 0777);
	parse->outfile = parse->fd;
	if (pwd1)
		free(pwd1);
	free(home);
}

void	handle_relative_path(char **pwd, t_parse *current_parse)
{
	char	*str;
	char	*temp;
	t_parse	*m_next;

	str = getcwd(NULL, 0);
	m_next = current_parse->next;
	*pwd = ft_strjoin(str, "/");
	free(str);
	temp = ft_strjoin(*pwd, m_next->text[0]);
	free(*pwd);
	*pwd = temp;
}

void	other_text_create_meta(t_parse *current_parse, t_shell *my_shell)
{
	t_parse	*n_parse;
	int		i;
	int		j;

	n_parse = current_parse->next;
	i = 0;
	while (current_parse->text[i])
		i++;
	while (n_parse && n_parse->type != 2)
	{
		j = 1;
		while (n_parse->text[j])
		{
			current_parse->text[i] = ft_strdup(n_parse->text[j]);
			i++;
			j++;
		}
		n_parse = n_parse->next;
	}
	current_parse->text[i] = NULL;
	other_out_files_meta(current_parse, my_shell);
}

int	create_in_files_meta(t_parse *parse, t_shell *my_shell)
{
	t_parse	*nparse;
	char	*pwd;
	char	*home;

	home = env_get(my_shell->env, "HOME");
	nparse = parse->next;
	if (!ft_strnstr(nparse->text[0], home, ft_strlen(home)))
		handle_relative_path(&pwd, parse);
	else
		pwd = ft_strdup(nparse->text[0]);
	if (!access(pwd, F_OK))
		nparse->fd = open(pwd, O_RDONLY, 0777);
	else
	{
		if (pwd)
			free(pwd);
		printf("minishell: %s: No such file or directory\n", nparse->text[0]);
		return (0);
	}
	if (pwd)
		free(pwd);
	free(home);
	parse->infile = nparse->fd;
	return (1);
}

void	create_out_file(t_parse *current_parse, t_parse *first_parse,
		t_shell *my_shell)
{
	char	*pwd;
	t_parse	*m_next;
	char	*home;

	home = env_get(my_shell->env, "HOME");
	m_next = current_parse->next;
	if ((m_next->type == 3 || m_next->type == 4))
		return (free(home), other_text_create_meta(current_parse, my_shell));
	if (!ft_strnstr(m_next->text[0], home, ft_strlen(home)))
		handle_relative_path(&pwd, current_parse);
	else
		pwd = ft_strdup(m_next->text[0]);
	if (current_parse->type == 4)
		m_next->fd = open(pwd, O_CREAT | O_RDWR | O_APPEND, 0777);
	else if (current_parse->type == 3)
		m_next->fd = open(pwd, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (current_parse->cmd)
		current_parse->outfile = m_next->fd;
	else if (first_parse->cmd)
		first_parse->outfile = m_next->fd;
	if (pwd)
		free(pwd);
	free(home);
}
