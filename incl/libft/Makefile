# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mschiman <mschiman@student.42wolfsburg.de> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/14 11:48:46 by mschiman          #+#    #+#              #
#    Updated: 2022/05/30 15:47:31 by mschiman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SRC =	ft_atoi.c \
		ft_bzero.c \
		ft_calloc.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit.c \
		ft_isprint.c \
		ft_iswhitespace.c \
		ft_itoa.c \
		ft_memccpy.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_memset.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_split.c \
		ft_strchr.c \
		ft_strdup.c \
		ft_strjoin.c \
		ft_strjoin_char.c \
		ft_strlcat.c \
		ft_strlcpy.c \
		ft_strlen.c \
		ft_strmapi.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_strrchr.c \
		ft_strtrim.c \
		ft_substr.c \
		ft_tolower.c \
		ft_toupper.c \
		ft_atoll.c

BONUS = ft_lstadd_back.c \
		ft_lstadd_front.c \
		ft_lstclear.c \
		ft_lstdelone.c \
		ft_lstiter.c \
		ft_lstlast.c \
		ft_lstnew.c \
		ft_lstsize.c

OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS:.c=.o)
CFLAGS = -Wall -Wextra -Werror -c
CC = gcc
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	ar -rcs $(NAME) $(OBJ)
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC)

bonus: $(BONUS_OBJ)
	ar -rcs $(NAME) $(BONUS_OBJ)

$(BONUS_OBJ): $(BONUS)
	$(CC) $(CFLAGS) $(BONUS)

clean:
	$(RM) $(OBJ) $(BONUS_OBJ)
	@rm -f *.o

fclean: clean
	$(RM) $(NAME)
	@rm -f *.a

re: fclean all

.PHONY: all clean fclean re