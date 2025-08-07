# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 16:50:13 by myli-pen          #+#    #+#              #
#    Updated: 2025/08/07 22:30:43 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=fdf

URL_MLX		=https://github.com/codam-coding-college/MLX42.git

LIBFT		=$(DIR_LIBFT)libft.a
MLX42		=$(DIR_MLX)build/libmlx42.a

CC			=cc
CFLAGS		=-Wall -Wextra -Werror -Wunreachable-code -O3
LDFLAGS		=-ldl -lglfw -pthread -lm
MAKEFLAGS	+= --no-print-directory

DIR_LIBFT	=$(DIR_LIB)libft/
DIR_MLX		=$(DIR_LIB)MLX42/
DIR_INC		=inc/
DIR_SRC		=src/
DIR_OBJ		=obj/
DIR_LIB		=lib/
DIR_DEP		=dep/

HEADERS		=$(addprefix -I , \
				$(DIR_INC) $(DIR_LIBFT)$(DIR_INC) $(DIR_MLX)include/MLX42/)
SRCS		=$(addprefix $(DIR_SRC), \
				main.c mesh.c parsing.c projection.c rendering.c \
				colors.c camera.c model.c camera_controls.c ui.c \
				input.c clipping.c depth.c)
OBJS		=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		=$(patsubst $(DIR_SRC)%.c, $(DIR_DEP)%.d, $(SRCS))

BLUE		=\033[1;34m
YELLOW		=\033[1;33m
GREEN		=\033[1;32m
RED			=\033[1;31m
COLOR		=\033[0m

all: $(LIBFT) $(MLX42) $(NAME)

$(DIR_OBJ):
	@mkdir -p $(DIR_LIB) $(DIR_OBJ) $(DIR_DEP)
	@echo "$(GREEN) [+]$(COLOR) created missing directories"

$(LIBFT):
	@echo "$(GREEN) [+]$(COLOR) compiling libft.a"
	@make -C $(DIR_LIBFT)

$(MLX42):
	@if [ ! -d "$(DIR_MLX)" ]; then \
		echo "$(BLUE) [~]$(COLOR) cloning to lib/ $(BLUE)$(URL_MLX)$(COLOR)"; \
		git clone --quiet $(URL_MLX) $(DIR_MLX); \
	fi
	@echo "$(GREEN) [+]$(COLOR) compiling mlx42.a"
	@cmake $(DIR_MLX) -B $(DIR_MLX)build > /dev/null
	@make -C $(DIR_MLX)build -j4 > /dev/null
	@echo "$(YELLOW) [✔] mlx42.a created$(COLOR)"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(MLX42) $(LDFLAGS)
	@echo "$(YELLOW) [✔] $(NAME) created$(COLOR)"

$(DIR_OBJ)%.o: $(DIR_SRC)%.c | $(DIR_OBJ)
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(HEADERS)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"

clean:
	@if [ -d "$(DIR_OBJ)" ]; then \
		rm -rf $(DIR_OBJ) $(DIR_DEP); \
		echo "$(RED) [-]$(COLOR) removed $(DIR_OBJ)"; \
		echo "$(RED) [-]$(COLOR) removed $(DIR_DEP)"; \
	fi
	@make -C $(DIR_LIBFT) clean

fclean: clean
	@if [ -d "$(DIR_MLX)" ]; then \
		rm -rf $(DIR_MLX); \
		echo "$(RED) [-]$(COLOR) removed $(DIR_MLX)"; \
	fi
	@make -C $(DIR_LIBFT) fclean
	@if [ -e "$(NAME)" ]; then \
		rm -f $(NAME); \
		echo "$(RED) [-]$(COLOR) removed $(NAME)"; \
	fi

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS) $(DEPS)

-include $(DEPS)
