/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 22:37:25 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/16 20:27:07 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define WIDTH 2560
# define HEIGHT 1440
# define WHITE 0xFFFFFFFF
# define BLACK 0x000000FF
# define ALTITUDE 100

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

typedef struct	s_mat4
{
	float	m[4][4];
}				t_mat4;

typedef struct	s_color
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;
}				t_color;

typedef struct	s_quad
{
	int	topLeft;
	int topRight;
	int	bottomLeft;
	int	bottomRight;
}				t_quad;

typedef struct	s_vertex
{
	t_vec3		pos;
	uint32_t	color;
}				t_vertex;

typedef struct	s_render_context
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_vector	*vertices;
	t_vector	*indices;
	t_vec2		rows_cols;
	t_vec2		offset;
	t_mat4		transform;
	float		scale;
	int			max_altitude;
	t_vertex	(*project)(t_vertex *v, float scale, t_mat4 transform);
}				t_render_context;

#endif
