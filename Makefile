# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pprussen <pprussen@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/14 11:48:46 by mschiman          #+#    #+#              #
#    Updated: 2022/09/08 12:38:42 by pprussen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = 	./src/main.c \
		./src/parse_input.c \
		./src/errors.c \
		./src/syntax_error_check.c \
		./src/syntax_error_check2.c \
		./src/fill_cmd_structures.c \
		./src/expand_variables.c \
		./src/put_temp_input_to_cmd.c \
		./src/put_temp_input_to_cmd_utils.c \
		./src/replace_str.c \
		./src/copy_env.c \
		./src/str_split.c \
		./src/str_split_utils.c \
		./src/execute_cmds.c \
		./src/create_env_from_list.c \
		./src/handle_here_doc.c \
		./src/pipes.c \
		./src/redir.c \
		./src/init.c \
		./src/expand_var_utils.c \
		./src/fd.c \
		./src/dir_list.c \
		./src/path.c \
		./src/inbuilts_functions/cd.c \
		./src/inbuilts_functions/exit.c \
		./src/inbuilts_functions/export.c \
		./src/inbuilts_functions/unset.c \
		./src/inbuilts_functions/find_inbuilts.c \
		./src/accurat_cleaner.c \
		./incl/leaks_checker.c

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -O0
LIBFT = ./incl/libft
OBJS = ${SRC:.c=.o}

$(NAME) : $(OBJS)
		@make  -C ./incl/libft
		@make  bonus -C ./incl/libft
		@make all -C ./incl/inbuilts_progs
		$(CC) $(CFLAGS) $(SRC) -L$(LIBFT) -lft -lreadline -o $(NAME)
		
all:	$(NAME)

clean:
	@make clean -C ./incl/libft
	@make clean -C ./incl/inbuilts_progs
	@rm -f ${OBJS}

fclean: clean
	-rm $(NAME)
#	-rm ./src/inbuilts/*.o
	-rm ./src/*.o
	-rm ./incl/libft/*.o
	-rm ./incl/inbuilts_progs/env/*.o
	-rm ./incl/inbuilts_functions/*.o
#	-rm ./src/utils/*.o
	@make fclean -C ./incl/libft
	@make fclean -C ./incl/inbuilts_progs

re: fclean all

.PHONY: clean fclean re re_bonus all