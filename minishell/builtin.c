/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 23:30:18 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 04:20:10 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"

bool	is_numeric(char *s)
{
	while ((9 <= *s && *s <= 13) || *s == 32)
		s++;
	if (*s == 43 || *s == 45)
		s++;
	while (48 <= *s && *s <= 57)
		s++;
	while ((9 <= *s && *s <= 13) || *s == 32)
		s++;
	if (*s != '\0')
		return (false);
	return (true);
}

void	execute_builtin_command(t_parse *parse, t_shell *my_shell)
{
	t_parse	*new_parse;

	my_shell->exec_status = 0;
	new_parse = parse;
	if ((ft_strcmp(new_parse->cmd, "echo") == 0) || (ft_strcmp(new_parse->cmd,
				"ECHO") == 0))
		exec_echo(new_parse, my_shell);
	if (ft_strcmp(new_parse->cmd, "cd") == 0)
		exec_cd(new_parse, my_shell);
	if ((ft_strcmp(new_parse->cmd, "pwd") == 0) || (ft_strcmp(new_parse->cmd,
				"PWD") == 0))
		exec_pwd(new_parse, my_shell);
	if (ft_strcmp(new_parse->cmd, "export") == 0)
		exec_export(new_parse, my_shell);
	if (ft_strcmp(new_parse->cmd, "unset") == 0)
		exec_unset(new_parse, my_shell, 0, 0);
	if (((ft_strcmp(new_parse->cmd, "env") == 0) || (ft_strcmp(new_parse->cmd,
					"ENV") == 0)))
		exec_env(my_shell);
}

int	is_builtin(t_parse *data)
{
	if (!data->cmd)
		return (0);
	if ((ft_strcmp(data->cmd, "echo") == 0) || (ft_strcmp(data->cmd,
				"ECHO") == 0))
		return (1);
	if ((ft_strcmp(data->cmd, "pwd") == 0) || (ft_strcmp(data->cmd,
				"PWD") == 0))
		return (1);
	if ((ft_strcmp(data->cmd, "env") == 0) || (ft_strcmp(data->cmd,
				"ENV") == 0))
		return (1);
	if ((ft_strcmp(data->cmd, "cd") == 0))
		return (1);
	if ((ft_strcmp(data->cmd, "export") == 0))
		return (1);
	if ((ft_strcmp(data->cmd, "unset") == 0))
		return (1);
	return (0);
}

void	builtin_exit(t_shell **my_shell)
{
	bool	status;
	int		exit_value;

	exit_value = 0;
	printf("exit\n");
	if (!(*my_shell)->parse->text || (*my_shell)->parse->type != '\0')
		exit(EXIT_SUCCESS);
	status = is_numeric((*my_shell)->parse->text[0]);
	if (status == false)
	{
		printf("bash: exit: %s: numeric argument required\n",
			(*my_shell)->parse->text[0]);
		exit_value = 255;
	}
	else if (status == true && !(*my_shell)->parse->text[1])
		exit_value = ft_atoi((*my_shell)->parse->text[0]);
	else
	{
		printf("bash: exit: too many arguments\n");
		(*my_shell)->exec_status = 1;
		return ;
	}
	free_init(&(*my_shell), 1);
	exit(exit_value);
}
