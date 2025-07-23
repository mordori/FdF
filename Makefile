# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 16:50:13 by myli-pen          #+#    #+#              #
#    Updated: 2025/07/23 06:32:39 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS	+= --no-print-directory
NAME		=fdf

URL_LIBFT	=https://github.com/mordori/libft.git
URL_MLX		=https://github.com/codam-coding-college/MLX42.git

LIBFT		=$(DIR_LIBFT)libft.a
MLX42		=$(DIR_MLX)build/libmlx42.a

CC			=cc
CFLAGS		=-Wall -Wextra -Werror -Wunreachable-code -O3
LDFLAGS		=-ldl -lglfw -pthread -lm
VECTOR_SIZE	=1024

DIR_LIBFT	=$(DIR_LIB)libft/
DIR_MLX		=$(DIR_LIB)MLX42/
DIR_INC		=include/
DIR_SRC		=src/
DIR_OBJ		=obj/
DIR_LIB		=lib/
DIR_DEP		=dep/

HEADERS		=$(addprefix -I , \
				$(DIR_INC) $(DIR_LIBFT)$(DIR_INC) $(DIR_MLX)$(DIR_INC)MLX42/)
SRCS		=$(addprefix $(DIR_SRC), \
				fdf.c fdf_utils.c mesh.c parsing.c projection.c rendering.c \
				rendering_utils.c camera.c model.c camera_controller.c)
OBJS		=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		=$(patsubst $(DIR_SRC)%.c, $(DIR_DEP)%.d, $(SRCS))

BLUE		=\033[1;34m
YELLOW		=\033[1;33m
GREEN		=\033[1;32m
RED			=\033[1;31m
COLOR		=\033[0m

all: $(DIR_OBJ) $(LIBFT) $(MLX42) $(NAME)

$(DIR_OBJ):
	@mkdir -p $(DIR_LIB) $(DIR_OBJ) $(DIR_DEP)
	@echo "$(GREEN) [+]$(COLOR) created missing directories"

$(LIBFT):
	@if [ ! -d "$(DIR_LIBFT)" ]; then \
		echo "$(BLUE) [~]$(COLOR) cloning to lib/ $(BLUE)$(URL_LIBFT)$(COLOR)"; \
		git clone --quiet $(URL_LIBFT) $(DIR_LIBFT); \
	fi
	@echo "$(GREEN) [+]$(COLOR) compiling libft.a"
	@make -C $(DIR_LIBFT) VECTOR_SIZE=8

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

$(DIR_OBJ)%.o: $(DIR_SRC)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(HEADERS) -D VECTOR_SIZE=$(VECTOR_SIZE)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"

clean:
	@if [ -d "$(DIR_OBJ)" ]; then \
		rm -rf $(DIR_OBJ) $(DIR_DEP); \
		echo "$(RED) [-]$(COLOR) removed $(DIR_OBJ)"; \
		echo "$(RED) [-]$(COLOR) removed $(DIR_DEP)"; \
	fi
	@if [ -d "$(DIR_LIBFT)" ]; then \
		make -C $(DIR_LIBFT) clean; \
	fi

fclean: clean
	@if [ -d "$(DIR_LIB)" ]; then \
		rm -rf $(DIR_LIB); \
		echo "$(RED) [-]$(COLOR) removed $(DIR_LIB)"; \
	fi
	@if [ -e "$(NAME)" ]; then \
		rm -f $(NAME); \
		echo "$(RED) [-]$(COLOR) removed $(NAME)"; \
	fi

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS) $(DEPS)

-include $(DEPS)
