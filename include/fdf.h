/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 22:37:25 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/18 07:12:52 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# define WIDTH 2560
# define HEIGHT 1440

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>

# include "MLX42.h"
# include "libft_io.h"
# include "libft_str.h"
# include "libft_utils.h"
# include "libft_math.h"
# include "libft_matrix.h"
# include "libft_mem.h"
# include "libft_vector.h"

typedef struct s_view
{
	t_vec3	eye;
	t_vec3	target;
	t_vec3	up;
}				t_view;

typedef struct s_matrices
{
	t_mat4	t;
	t_mat4	r;
	t_mat4	s;
	t_mat4	m;
	t_mat4	v;
	t_mat4	p;
	t_mat4	mvp;
}				t_matrices;

typedef struct	s_context
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_vector	*verts;
	t_vector	*tris;
	t_vec2		rows_cols;
	t_vec2		offset;
	t_vec3		center;
	bool		alt_color;
	float		scale;
	int			max_alt;
	float		fov;
	float		aspect;
	float		near;
	float		far;
	t_transform	transform;
	t_view		view;
}				t_context;

int		parse_map(char *map, t_vector *verts, t_vec2 *rows_cols, int *alt);
void	on_close(void* param);
void	on_resize(int width, int height, void *param);
void	ft_mlx_error(mlx_t *mlx);
//t_vertex	proj_iso(t_vertex *v, float scale, t_mat4 transform);
t_mat4	proj_persp(float fov, float aspect, float near, float far);
//void	compute_scale(t_context *ctx);
//void	get_bounds(t_context *ctx);
t_vertex	*make_vert(float x, float y, float z, uint32_t color);
void	make_triangles(t_vector *tris, t_vec2 rows_cols);
t_vec3	*make_tri(int x, int y, int z);
void	clear_image(mlx_image_t *img, uint32_t color);
uint32_t	lerp_color(uint32_t c1, uint32_t c2, float t);
void	render(void *param);
void	fdf_free(t_vector *verts, t_vector *tris, t_context *ctx);
bool	mvp(t_vertex *vert, t_context *ctx);

#endif
