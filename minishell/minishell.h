/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mehmyild <mehmyild@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 18:04:12 by mehmyild          #+#    #+#             */
/*   Updated: 2024/03/24 03:50:02 by mehmyild         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"

# define ALLOC_ERROR "Error = Allocation error\n"
# define W_ARGS "Error = Cannot accept any argument\n"
# define QUATE_ERROR "Error = Quate error\n"
# define BAD_EXIT 1
# define SYNTX_ERROR "syntax error near unexpected token\n"
# define PIPE 2
# define GREATER 4
# define HEREDOC 1
# define GREAT 3
# define LESS 5
# define STDINN 0
# define STDOUT 1
# define STDERR 2

typedef struct s_parse
{
	char			*cmd;
	char			**text;
	int				type;
	int				outfile;
	int				infile;
	int				fd;
	int				pid;
	int				control;
	int				std_in;
	struct s_parse	*next;
}				t_parse;

typedef struct s_env
{
	char	*key;
	char	*value;
}				t_env;

typedef struct s_lexer
{
	t_list	*lex;
}				t_lexer;

typedef struct s_shell
{
	t_list		*env;
	t_parse		*parse;
	char		*cmd;
	char		*heredoc;
	t_lexer		*lexer_lst;
	int			exec_status;
}				t_shell;

extern int		g_heredoc_sig;

int		is_space(char *cmd);
int		quate_check(char *cmd, char c, char d);
int		get_env(t_shell **my_shell, char **env);
char	*get_home(t_shell *my_shell);
void	token_type(t_parse **parse, const char *str);
void	dollar_countine(char *cmd, size_t *i, size_t *j);
void	edit_env_(t_list *node, char *key, char *value, t_shell *my_shell);
void	edit_env(t_list *node, char *key, char *value, t_shell *my_shell);
void	create_out_file(t_parse *current_parse, t_parse *first_parse,
			t_shell *my_shell);
int		create_in_files_meta(t_parse *parse, t_shell *my_shell);
int		ft_strchrindex_0(char *s, int c);
int		ft_strchrindex(char *s, int c);
void	remove_quotes_in_str(char *str);
void	remove_quote(t_parse *parse);
void	flag_free(t_parse *parse);
int		is_valid_other(char *a, char *b);
void	del_env(void *content);
char	*ft_strjoin_char(char *s1, char s2);
void	signal_control(void);
void	free_init(t_shell **my_shell, int control);
int		lexer_start(t_shell *my_shell, char *cmd, t_lexer **lexer_lst);
int		lexer(t_shell *my_shell);
int		expander(t_shell *my_shell);
int		expand_dollar_variable_continue(t_shell *shell, t_list *lex,
			char **temp, char *before);
int		expand_question_mark(t_shell *shell, t_list *lex, char **temp,
			char *before);
char	*heredoc_control(t_shell *my_shell, char *delimiter, int *flag);
int		ft_strcmp(const char *s1, const char *s2);
int		parser(t_shell *my_shell, char **env);
int		create_files(t_shell *my_shell);
void	exec(char **env, t_shell *my_shell);
void	loop_heredoc(t_shell *my_shell);
void	set_heredoc(int sig);
void	killer(int sig);
void	sigint_handler(int signum);
int		is_builtin(t_parse *data);
void	builtin_exit(t_shell **my_shell);
char	*add_char(char *str, char a);
int		single_or_multi_command(t_shell *my_shell);
void	execute_builtin_command(t_parse *parse, t_shell *my_shell);
void	exec_echo(t_parse *data, t_shell *my_shell);
void	exec_cd(t_parse *data, t_shell *my_shell);
void	exec_pwd(t_parse *data, t_shell *my_shell);
int		exec_export(t_parse *data, t_shell *my_shell);
void	exec_unset(t_parse *data, t_shell *my_shell, int i, int x);
void	exec_env(t_shell *my_shell);
void	ft_newlstiter(t_list *lst, void (*f)(void *,
				t_shell *), t_shell *my_shell);
char	**get_args(t_parse *parse);
void	free_text(char ***text);
void	exec_others(t_parse *parse, char **env, int *fd, t_shell *my_shell);
void	create_dup_two(t_parse *parse, int *fd);
void	clear_pipe(int *fd);
void	wait_all(t_shell *my_shell);
char	*env_get(t_list *env, char *our_env);
void	handle_relative_path(char **pwd, t_parse *current_parse);
void	create_dup_one(t_parse *parse, int *fd);
void	create_dup(t_shell *my_shell, t_parse *parse);
void	free_loop(int control, t_shell *m_shell);
void	free_parser(t_shell *my_shell);
t_parse	*parse_init(t_list *lex);
t_parse	*_next_command(t_parse **parse);
void	non_old(t_shell *my_shell, char *str, char *targetkey);
int		is_count_odd(char *before, char c);
int		check_quote(char *before, char *after);

#endif
