/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 22:37:25 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 20:36:46 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# ifndef WIDTH
#  define WIDTH 1920
# endif

# ifndef HEIGHT
#  define HEIGHT 1080
# endif

# define ZOOM_SENS 0.0018f
# define PAN_SENS 0.0006f
# define ORBIT_SENS 0.0025f

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

typedef enum e_space
{
	OBJECT,
	WORLD,
	VIEW,
	CLIP,
	NDC,
	SCREEN
}				t_space;

typedef enum e_proj
{
	ISOMETRIC,
	ORTHOGRAPHIC,
	PERSPECTIVE
}				t_proj;

typedef enum e_color_mode
{
	DEFAULT,
	AMAZING
}				t_color_mode;

typedef enum e_spin_mode
{
	OFF,
	ON
}				t_spin_mode;

typedef struct s_cam
{
	t_vec3	eye;
	t_vec3	target;
	t_vec3	up;
	float	distance;
	float	yaw;
	float	pitch;
	float	fov;
	float	aspect;
	float	near;
	float	far;
	float	ortho_size;
	t_proj	projection;
	bool	zooming;
	bool	panning;
	bool	orbiting;
}				t_cam;

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

typedef struct s_context
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	float			*z_buf;
	t_vector		*verts;
	t_vector		*tris;
	t_vec2i			rows_cols;
	t_vec2i			alt_min_max;
	t_vec3			center;
	t_vec3			bounds;
	t_transform		transform;
	t_cam			cam;
	t_color_mode	color_mode;
	t_spin_mode		spin_mode;
	uint32_t		color;
	uint32_t		color1;
	uint32_t		color2;
	double			time_rot;
	t_matrices		m;
}				t_context;

int			parse_map(char *map, t_vector *verts, t_vec2i *rows_cols);
void		resize(int width, int height, void *param);
void		ft_error(mlx_t *mlx, char *message);
bool		make_triangles(t_vector *tris, t_vec2i rows_cols);
void		clear_image(t_context *ctx, uint32_t color);
void		render(void *param);
void		fdf_free(t_vector *verts, t_vector *tris, t_context *ctx, char *msg);
bool		project_to_screen(t_vertex *vert, t_context *ctx);
void		update_camera(t_cam *cam);
void		init_camera(t_context *ctx);
void		update_ui(t_context *ctx);
void		frame(t_context *ctx);
void		reset_transforms(t_context *ctx);
void		control_camera(void *param);
t_vec3		*make_tri(int x, int y, int z);
t_mat4		model_matrix(t_context *ctx);
t_mat4		view_matrix(t_cam cam);
t_mat4		proj_persp(t_cam cam);
t_mat4		proj_ortho(t_cam cam);
uint32_t	rainbow_rgb(double t);
uint32_t	lerp_color(uint32_t c1, uint32_t c2, float t);
t_vertex	*make_vert(float x, float y, float z, uint32_t color);
int			wrap_m_x(t_context *ctx, t_vec2i *pos);
int			wrap_m_y(t_context *ctx, t_vec2i *pos);
void		key_hook(mlx_key_data_t keydata, void *param);
void		translate_rotate(t_context *ctx);
void		compute_bounds(t_context *ctx, t_space space,
				size_t i, t_vertex *v);
void		initialize(char *file, t_context **ctx,
				mlx_t *mlx, mlx_image_t *img);
uint32_t	rgba_to_abgr(uint32_t rgba);
bool 		liang_barsky_clip(t_vertex *v0, t_vertex *v1);
bool 		liang_barsky_screen(t_context *ctx, t_vec4 *v);
bool		depth_test(t_context *ctx, t_vertex v0, t_vertex v1, t_vec3 t);
void		control_fov(t_context *ctx);
void		compute_distance(t_context *ctx);

#endif
