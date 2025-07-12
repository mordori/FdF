/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 22:37:25 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/12 01:05:53 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define WIDTH 512
# define HEIGHT 512
# define WHITE 0xFFFFFFFF
# define TILE_WIDTH 32
# define TILE_HEIGHT 16
# define ALTITUDE_SCALE 2

# include <stdlib.h>	// exit
# include <stdio.h>		// perror
# include <fcntl.h>		// open
# include <math.h>

# include "MLX42.h"
# include "libft_io.h"		// ft_printf, get_next_line, ft_split
# include "libft_str.h"		// ft_split
# include "libft_utils.h"	// ft_atoi_base
# include "libft_mem.h"		// ft_memset
# include "libft_vector.h"	// ft_vector

typedef struct	s_vec2
{
	int	x;
	int	y;
}				t_vec2;

typedef struct	s_vec3
{
	int	x;
	int	y;
	int	z;
}				t_vec3;

typedef struct	s_vertex
{
	t_vec3		pos;
	uint32_t	color;
}				t_vertex;

typedef struct	s_render_context
{
	mlx_image_t	*img;
	t_vector	*vertices;
	t_vector	*indices;
	t_vec2		rows_cols;
	t_vertex	(*project)(t_vertex *v);
}				t_render_context;

#endif
