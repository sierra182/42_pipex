# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svidot <svidot@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/04 06:54:46 by svidot            #+#    #+#              #
#    Updated: 2023/12/04 06:54:46 by svidot           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

LIBFT_DIR = ft_printf/libft
FT_PRINTF_DIR = ft_printf
SRC_DIR = sources
HEADERS_DIR = include

BONUS_DIR = bonus
SRCS_BONUS_DIR = $(BONUS_DIR)/sources
GNL_DIR = $(BONUS_DIR)/gnl

CC = cc
CFLAGS = -I$(LIBFT_DIR) -I$(FT_PRINTF_DIR) -I$(HEADERS_DIR) -Wall -Wextra -Werror
CFLAGS_BONUS = -I$(GNL_DIR) -DEN_BONUS

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/setup.c $(SRC_DIR)/parsing_path.c $(SRC_DIR)/parsing_build.c $(SRC_DIR)/parsing_invrt.c \
$(SRC_DIR)/parsing_qute.c $(SRC_DIR)/parsing_spce.c $(SRC_DIR)/parsing_utils.c

SOURCES_BONUS = $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c $(SRCS_BONUS_DIR)/main.c $(SRCS_BONUS_DIR)/setup.c \
$(SRCS_BONUS_DIR)/parsing_path.c $(SRCS_BONUS_DIR)/parsing_build.c $(SRCS_BONUS_DIR)/parsing_invrt.c $(SRCS_BONUS_DIR)/parsing_qute.c \
$(SRCS_BONUS_DIR)/parsing_spce.c $(SRCS_BONUS_DIR)/parsing_utils.c 

OBJECTS = $(SOURCES:.c=.o)
OBJECTS_BONUS = $(SOURCES_BONUS:.c=_bonus.o)

LIBFTPRINTF = $(FT_PRINTF_DIR)/libftprintf.a
NAME = pipex
NAME_BONUS = $(BONUS_DIR)/pipex

HEADER = $(HEADERS_DIR)/bonus.h $(HEADERS_DIR)/setup.h $(HEADERS_DIR)/parsing_build.h $(HEADERS_DIR)/parsing_invrt.h \
$(HEADERS_DIR)/parsing_qute.h $(HEADERS_DIR)/parsing_spce.h $(HEADERS_DIR)/parsing_utils.h

HEADERS_BONUS = $(GNL_DIR)/get_next_line.h 

.PHONY: all ft_printf bonus clean fclean re intro l newline backline emoticon

$(SRC_DIR)/%.o : $(SRC_DIR)/%.c $(LIBFTPRINTF) $(HEADER)
	@echo "\033[0;32m compiling $(NAME) object $<...\033[0m" 🚀
	@$(CC) $(CFLAGS) $< -c -o $@

$(BONUS_DIR)/%_bonus.o : $(BONUS_DIR)/%.c $(LIBFTPRINTF) $(HEADER) $(HEADERS_BONUS)
	@echo "\033[0;32m compiling $(NAME) object bonus $<...\033[0m" 🚀
	@$(CC) $(CFLAGS) $(CFLAGS_BONUS) $< -c -o $@	

all: intro ft_printf $(NAME) emoticon		

l: ft_printf $(NAME) 
	
ft_printf: emoticon
	@$(MAKE) -s -C $(LIBFT_DIR) bonus 
	@$(MAKE) -s -C $(FT_PRINTF_DIR) 

$(NAME) : $(OBJECTS) 
	@echo "\n\033[0;32m linking $(NAME) objects with $(LIBFTPRINTF)...\033[0m 🚀\n\n 💗 💎 💎 💗\n"
	@$(CC) $(OBJECTS) $(LIBFTPRINTF) -o $@

bonus: ft_printf $(NAME_BONUS)
	@echo " 💎 🧯 🔥 😵\n"

$(NAME_BONUS) : $(OBJECTS_BONUS)
	@echo "\n\033[0;32m linking $(NAME) objects with flag and objects bonus with $(LIBFTPRINTF) to $(NAME_BONUS) \033[0m 🚀\n"
	@$(CC) $(OBJECTS_BONUS) $(LIBFTPRINTF) -o $@

emoticon:
	@echo "\n 💗 😀 😃 😍\n"

newline: 
	@echo ""

backline: 
	@echo "\033[A\033[A"

intro:
	clear
	@sleep 1
	@echo -n " Wake"
	@sleep .4
	@echo " up...\n"
	@sleep .1
	@echo -n " "
	@letters="The Makefile has you..."; \
	echo "\033[1;92m$$letters\033[0m" | awk '{split($$0, chars, ""); \
	for (i = 1; i <= length($$0); i++) \
	{printf "%s", chars[i]; system("sleep 0.2");}} \
	system("sleep 1");' 
	@words="\n\n\tFollow \n\t\tthe \t\b\b\033[0;96mColored! \
	\n\n\t\t\b\b\b\b\b\b\033[1;95mUnicorne!\033[0m...🦄\n\n"; \
	for j in $${words}; do \
		echo -n "$$j"; \
		sleep .4; \
	done
	@sleep .4
	@cat mfile_design	

clean:
	@echo "\n cleanning $(NAME) objects 🧻"
	@rm -f $(OBJECTS) $(OBJECTS_BONUS);
	@$(MAKE) -C $(FT_PRINTF_DIR) clean -s
	@echo ""

fclean: 
	@echo "\n cleanning $(NAME) objects 🧻"
	@rm -f $(OBJECTS) $(OBJECTS_BONUS);
	@echo " cleanning $(NAME) 🚽" 
	@rm -f $(NAME) $(NAME_BONUS)
	@$(MAKE) -C $(FT_PRINTF_DIR) fclean -s
	@echo ""

re: fclean backline l
