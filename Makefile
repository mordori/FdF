# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 16:50:13 by myli-pen          #+#    #+#              #
#    Updated: 2025/06/30 17:47:29 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL = all
NAME = fdf
MLX42 = libmlx42.a
LIBFT = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
MAKEFLAGS += --no-print-directory
DIR_LIBFT = libft
DIR_SRC = src
DIR_OBJ = obj
DIR_DEP = dep

SRCS = $(addprefix $(DIR_SRC)/, \
		)
OBJS = $(patsubst $(DIR_SRC)/%.c, $(DIR_OBJ)/%.o, $(SRCS))
DEPS = $(patsubst $(DIR_SRC)/%.c, $(DIR_DEP)/%.d, $(SRCS))

all: $(LIBFT) $(MLX42) $(NAME)

$(LIBFT):
	@make -C $(DIR_LIBFT)
	@mkdir -p $(DIR_OBJ) $(DIR_DEP)

$(NAME): $(MLX42) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) main.c $(OBJS) $(LIBFT) $(MLX42) \
	-Iinclude -ldl -lglfw -pthread -lm
	@echo "\033[1;33m [✔] $(NAME) created \033[0m"

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ \
	-MMD -MP -MF $(patsubst $(DIR_OBJ)/%.o, $(DIR_DEP)/%.d, $@)
	@echo "\033[1;32m [+]\033[0m compiling $@"

clean:
	@rm -rf $(DIR_OBJ) $(DIR_DEP)
	@echo "\033[1;31m [-]\033[0m removed ./obj/"
	@echo "\033[1;31m [-]\033[0m removed ./dep/"

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[1;31m [-]\033[0m removed $(NAME)"
	@make fclean -C $(DIR_LIBFT)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS) $(DEPS)

-include $(DEPS)
