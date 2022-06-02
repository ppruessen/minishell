# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/03 12:09:19 by pprussen          #+#    #+#              #
#    Updated: 2022/05/29 11:20:34 by mschiman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = gcc
CFLAGS = -Wall -Werror -Wextra

PWD = pwd
ENV = env
ECHO_MINI = echo

PWD_MAIN = pwd.c
ENV_MAIN = env.c
ECHO_MAIN = echo.c

LIBFT = ../libft
LIBFT_NAME = libft.a

all: $(PWD) $(ENV) $(ECHO_MINI)

$(PWD) : pwd.o
	$(CC) $(CFLAGS) $(PWD_MAIN) -L$(LIBFT) -lft -o $(PWD)
	@rm -f pwd.o

$(ENV) : env.o
	$(CC) $(CFLAGS) $(ENV_MAIN) -L$(LIBFT) -lft -o $(ENV)
	@rm -f env.o

$(ECHO_MINI) : echo.o
	$(CC) $(CFLAGS) $(ECHO_MAIN) -L$(LIBFT) -lft -o $(ECHO_MINI)
	@rm -f echo.o

clean:
	@rm -f echo.o
	@rm -f env.o
	@rm -f pwd.o

fclean : clean
	@rm -f ${ENV}
	@rm -f ${PWD}
	@rm -f ${ECHO_MINI}

re:	fclean all

re_bonus: fclean bonus

.PHONY: clean fclean re re_bonus