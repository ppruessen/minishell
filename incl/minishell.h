/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 13:09:10 by mschiman          #+#    #+#             */
/*   Updated: 2022/09/08 12:39:33 by pprussen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// GLOBAL VARIABEL NEEDS TO BE REMOVED BEFORE SUBMITTING
int	debug_mode;
void check_leaks();

# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include "./libft/libft.h"

/* BOOLEAN */
# define TRUE 1
# define FALSE 0

/* Prompt */
# define PAMELA_PROMPT "\033[0;35mPaMela <3 \033[0m"

/* Quotes */
# define S_QUOTES 1
# define D_QUOTES 2
# define OPEN -1
# define CLOSED 0

/* Redirections */
# define NOT_SET 0
/* in */
# define FROM_FILE 1
# define FROM_HERE_DOC 2
/* out */
# define INTO_FILE 1
# define INTO_FILE_APPEND 2

/* Used for inbuilt attributes */
# define IS_INBUILT 1
# define IS_NO_INBUILT 0

/* PIPES */
# define READ 0
# define WRITE 1

/* INBUILTS */
# define NOT_SET 0
# define EXIT 1
# define ECHO 2
# define ENV 3
# define PWD 4
# define EXPORT 5
# define UNSET 6
# define CD 7

/* ERROR CODES */
# define OPEN_QUOTES 0
# define DOUBLE_PIPES 1
# define SEMICOLON 2
# define AMPERSAND 3
# define BRACES 4
# define ASTERISK 5
# define EXCLAMATION 6
# define TOO_MANY_REDIR_IN_ONE 10
# define TOO_MANY_REDIR_IN_TWO 11
# define TOO_MANY_REDIR_OUT_ONE 15
# define TOO_MANY_REDIR_OUT_TWO 16
# define UNEXPECTED_TOKEN_NL 20
# define UNEXPECTED_TOKEN_PIPE 21
# define FILE_NOT_FOUND 22
# define CMD_NOT_FOUND 23

/* Structs */
typedef struct s_cmd
{
	char	**cmd;
	char	**cmd_esc;
	int		nb_cmds;
	int		inbuilt;
	int		read_from_file;
	int		write_to_file;
	int		read_from_pipe;
	int		write_to_pipe;
	int		fd_to_read;
	int		fd_to_write;
	char	*filename_to_read;
	char	*filename_to_write;
	char	*limiter;
}	t_cmd;

typedef struct s_var
{
	int		argc;
	char	**argv;
	char	**env;
	t_list	*env_list;
	char	**dir_list;
	char	pwd[500];
	char	*path;
	char	*prompt;
	char	*input;
	char	*input_escape;
	char	*t_input;
	char	*t_escape;
	int		index;
	t_cmd	**cmds;
	int		(*fd)[2];
	int		squotes;
	int		dquotes;
	int		pipes;
	char	*dollar_var;
	char	*dollar_esc;
	char	*dollar_value;
	char	*dollar_esc_value;
	int		cmd_check;
}	t_var;

typedef struct	s_replace
{
	int		i;
	int		j;
	int		start;
	int		end;
	char	*new_str;
	int		strlen_full;
	int		strlen_old_part;
	int		strlen_new_part;
	int		strlen_new_str;
}	t_replace;

typedef struct	s_split
{
	int		start;
	int		end;
}	t_split;

typedef struct	s_redir
{
	char	*input;
	char	*escape;
	int		start;
	int		i;
}	t_redir;

typedef struct	s_count
{
	int		i;
	int		count;
}	t_count;

/* -------- Global Variable(s) -------- */
int		g_status;

/* -------- Functions --------- */
/* src/parse_input.c */
int		parse_input(t_var *var);

/* src/errors.c */
int		print_error(t_var *var, int error_code);
void	print_file_error(t_var *var, int error_code);
void	print_cmd_error(t_var *var, int error_code, char *cmd);

/* src/syntax_error_check.c */
void	syntax_error_check(t_var *var);

/* src/syntax_error_check2.c */
int	syntax_error_check_redir(t_var *var);

/* src/fill_cmd_structures.c */
char	*get_next_word(char *full_str, char *esc_str, int start);
void	fill_cmd_structures(t_var *var);

/* src/expand_variables.c */
void	expand_variables(t_var *var);

/*src/expand_var_utils.c*/
char	*expand_env(t_var *var);
char	*expand_special_dollar(t_var *var);
int		mark_variables_to_expand(t_var *var);

/* src/put_temp_input_to_cmd.c */
int		word_runner(char *input, char *escaped, t_count *c);
void	put_temp_input_to_cmd(t_var *var, t_cmd *cmd);

/* src/put_temp_input_to_cmd_utils.c */
int	next_char_tester(char *input, char *escaped, t_count *cnt, char c);
int	next_char_runner(char c, char *input, char *escaped, t_count *cnt);

/* src/copy_env.c */
void	copy_env(t_var *var, char **env);

/* src/replace_str.c */
char	*replace_str(char *full_str, char *old_part, char *new_part);

/* src/str_split.c */
void	str_split(t_var *var, t_cmd *cmd, size_t word_num);

/* src/str_split_utils.c */
int	find_start(t_var *var, int start);
int	find_end(t_var *var, int end);

/*incl/dir_list.c*/
void	free_dir_list_cmd(char **dir_list, char *cmd);
void	gen_dir_list(t_var *var);

/* src/execute_cmds.c */
void	execute_cmds(t_var *var);

/* src/create_env_from_list.c */
char	**create_env_from_list(t_list *env_list);

/*src/fd.c*/
void	open_filedescriptors(t_var *var);
void	open_read_from_file(t_cmd *cmd);
void	open_write_to_file(t_cmd *cmd);

/*src/redir.c*/
int		set_redirections(char *temp_input, char *temp_escaped, t_cmd *cmd, t_var *var);
void	open_redirections(t_cmd *cmd);

/*src/path.c*/
char	*create_rel_path(char *cmd_path);
void	get_path(t_var *var);
char	*check_and_set_path(char *cmd, t_var *var);
char	*set_inbuilt_path(t_cmd *cmd, t_var *var);
void	set_cmd_path(t_cmd *cmd, t_var *var);

/*src/pipes.c*/
void	set_pipe_status(t_var *var, t_cmd *cmd, int cmd_nb);
void	close_all_read_pipes(t_var *var);
void	set_pipes_in_child(t_var *var, t_cmd *cmd, int i);
void	close_pipes_in_parent(t_var *var, int i);

/*src/init.c*/
void	init_cmd(t_cmd *cmd);

/* incl/inbuilts_functions */
/* export.c*  */
void	export_var(t_var *var, char **cmd, char **cmd_esc);
/* exit.c */
void	clean_up(t_var *var);
void	ms_exit(t_var *var, char **cmd);
void	clean_env(t_var *var);
/* unset.c */
void	unset_env(t_var *var, char **cmd);
/* cd.c */
void	change_directory(char **str);
/* find_inbuilts.c */
void	find_inbuilts(t_var *var, t_cmd *cmd, char *str);

/* src/handle_here_doc.c */
void	handle_here_doc(t_cmd *cmd);

/* accurat_cleaner.c */
void	accurat_var_cleaner(t_var *var);
void	accurat_env_cleaner(t_var *var);
void	accurat_finder(t_var *var);
#endif