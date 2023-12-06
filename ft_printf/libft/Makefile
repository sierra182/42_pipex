# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svidot <svidot@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/04 06:56:29 by svidot            #+#    #+#              #
#    Updated: 2023/12/04 07:08:59 by svidot           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Werror -Wextra -I.
NAME = libft.a
HEADER = libft.h
SOURCES = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_toupper.c ft_tolower.c \
			ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_strchr.c \
			ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
			ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
			ft_putendl_fd.c ft_putnbr_fd.c ft_isspace.c ft_wc.c ft_strcmp.c
OBJECTS = $(SOURCES:.c=.o)
SOURCES_BONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c \
				ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c
OBJECTS_BONUS = $(SOURCES_BONUS:.c=.o)
BONUS_FLAG = 0
.PHONY: all clean fclean re bonus 

all: $(NAME)

%.o : %.c $(HEADER)
	@echo "\033[0;32m compiling libft object $<...\033[0m" 🚀
	@$(CC) $(CFLAGS) $< -c -o $@

$(NAME) :: $(OBJECTS)
	@echo "\n\033[0;32m adding libft object $? to $@ ...\033[0m 🚀\n"
	@ar rcs $@ $?

change_flag:
	$(eval BONUS_FLAG = 1)

bonus: change_flag $(NAME)
	
$(NAME) :: $(OBJECTS_BONUS)
	@if [ $(BONUS_FLAG) -eq 1 ]; then \
		echo "\n\033[0;32m adding libft objects bonus $? to $@ ...\033[0m" 🚀; \
		ar rcs $@ $?; \
	fi

clean:
	@echo " cleanning libft objects 🧻"
	@rm -f $(OBJECTS) $(OBJECTS_BONUS)

fclean: clean
	@echo " cleanning libft.a 🚽"
	@rm -f $(NAME)

re: fclean all
