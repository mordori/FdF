# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/02 20:19:00 by myli-pen          #+#    #+#              #
#    Updated: 2025/08/05 01:08:07 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS	+= --no-print-directory
NAME		=libft.a

CC			=cc
CFLAGS		=-Wall -Wextra -Werror -Wunreachable-code -O3 -MMD -MP

DIR_INC		=inc/
DIR_SRC		=src/
DIR_OBJ		=obj/
DIR_DEP		=dep/

HEADERS		=-I $(DIR_INC)
SRCS		=$(addprefix $(DIR_SRC), \
			ft_atoi.c ft_isalpha.c ft_itoa.c ft_memmove.c ft_putnbr_fd.c \
			ft_bzero.c ft_isascii.c ft_memchr.c ft_memset.c ft_toupper.c \
			ft_calloc.c ft_isdigit.c ft_memcmp.c ft_putchar_fd.c \
			ft_isalnum.c ft_isprint.c ft_memcpy.c ft_putendl_fd.c \
			ft_strdup.c ft_strlcpy.c ft_strnstr.c ft_tolower.c \
			ft_putstr_fd.c ft_striteri.c ft_strlen.c ft_strrchr.c \
			ft_split.c ft_strjoin.c ft_strmapi.c ft_strtrim.c \
			ft_strchr.c ft_strlcat.c ft_strncmp.c ft_substr.c \
			ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c \
			ft_lstclear_bonus.c ft_lstdelone_bonus.c \
			ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
			ft_lstnew_bonus.c ft_lstsize_bonus.c ft_printf.c ft_uitoa.c \
			ft_countdigits.c ft_strchrdup.c ft_get_next_line.c ft_vector.c \
			ft_vector_utils.c ft_math.c ft_matrix.c ft_matrix_transforms.c \
			ft_vec4.c ft_vec3.c ft_vec3_2.c ft_matrix_utils.c \
			ft_vec2.c ft_vec4_2.c ft_vec2i.c ft_vec2i_2.c ft_math_2.c)
OBJS		=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		=$(patsubst $(DIR_SRC)%.c, $(DIR_DEP)%.d, $(SRCS))

YELLOW		=\033[1;33m
GREEN		=\033[1;32m
RED			=\033[1;31m
COLOR		=\033[0m

all: $(NAME)

$(DIR_OBJ):
	@mkdir -p $(DIR_LIB) $(DIR_OBJ) $(DIR_DEP)

$(NAME): $(OBJS)
	@ar -rcs $(NAME) $(OBJS)
	@echo "$(YELLOW) [✔] libft.a created $(COLOR)"

$(DIR_OBJ)%.o: $(DIR_SRC)%.c | $(DIR_OBJ)
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(HEADERS)

clean:
	@if [ -d "$(DIR_OBJ)" ]; then rm -rf $(DIR_OBJ) $(DIR_DEP); \
		echo "$(RED) [-]$(COLOR) removed libft/$(DIR_OBJ)"; \
		echo "$(RED) [-]$(COLOR) removed libft/$(DIR_DEP)"; \
	fi

fclean: clean
	@if [ -e "$(NAME)" ]; then rm -f $(NAME); \
		echo "$(RED) [-]$(COLOR) removed libft.a"; \
	fi

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS) $(DEPS)

-include $(DEPS)
