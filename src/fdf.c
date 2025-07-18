/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:19:35 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/18 07:26:54 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	input(mlx_key_data_t keydata, void *param);
static inline int	initialize(char *file, t_context **ctx);
static inline void	init_context(t_context *ctx, mlx_t *mlx, mlx_image_t *img);
static inline void	controls(void *param);
static inline void	loop(void *param);

int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_context	*ctx;

	if (argc != 2)
		return (EXIT_FAILURE);
	mlx_set_setting(MLX_MAXIMIZED, true);
	mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
		ft_mlx_error(NULL);
	img  = mlx_new_image(mlx, mlx->width, mlx->height);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_mlx_error(mlx);
	if (initialize(argv[1], &ctx) == ERROR)
		ft_mlx_error(mlx);
	init_context(ctx, mlx, img);
	mlx_key_hook(mlx, input, ctx);
	mlx_loop_hook(mlx, loop, ctx);
	mlx_close_hook(mlx, on_close, ctx);
	mlx_resize_hook(mlx, on_resize, ctx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	fdf_free(ctx->verts, ctx->tris, ctx);
	return (EXIT_SUCCESS);
}

static inline void	loop(void *param)
{
	t_context	*ctx;

	ctx = param;
	controls(ctx);
	render(ctx);
}

static inline int	initialize(char *file, t_context **ctx)
{
	t_vector	*verts;
	t_vector	*tris;
	int			max_alt;
	t_vec2		rows_cols;

	verts = malloc(sizeof (t_vector));
	tris = malloc(sizeof (t_vector));
	*ctx = malloc(sizeof (t_context));
	verts->items = NULL;
	tris->items = NULL;
	if (!verts || !tris || !*ctx)
		return (fdf_free(verts, tris, *ctx), ERROR);
	max_alt = INT32_MIN;
	if (!vector_init(verts, true))
		return (fdf_free(verts, tris, *ctx), ERROR);
	if (parse_map(file, verts, &rows_cols, &max_alt) == ERROR)
		return (fdf_free(verts, tris, *ctx), ERROR);
	if (!vector_init(tris, true))
		return (fdf_free(verts, tris, *ctx), ERROR);
	make_triangles(tris, rows_cols);
	(*ctx)->verts = verts;
	(*ctx)->tris = tris;
	(*ctx)->rows_cols = rows_cols;
	(*ctx)->max_alt = max_alt;
	return (true);
}

static inline void	init_context(t_context *ctx, mlx_t *mlx, mlx_image_t *img)
{
	t_vec3	eye = {15, 0, 20};
	t_vec3	target = {0, 0, -1.0f};
	t_vec3	up = {0, 1, 0};

	ctx->transform.pos = (t_vec3){0.0f, 0.0f, 0.0f};
	ctx->transform.rot = (t_vec3){0.0f, 0.0f, 0.0f};
	ctx->transform.scale = (t_vec3){1.0f, 1.0f, 1.0f};

	ctx->mlx = mlx;
	ctx->img = img;
	ctx->fov = M_PI / 3.0f;
	ctx->aspect = (float)mlx->width / mlx->height;
	ctx->near = 0.1f;
	ctx->far = 100.0f;
	ctx->scale = 1.0f;
	ctx->alt_color = false;
	ctx->view.eye = eye;
	ctx->view.target = target;
	ctx->view.up = up;
}

static inline void	input(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (!ctx->mlx)
		return ;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
	if (keydata.key == MLX_KEY_P && keydata.action == MLX_RELEASE)
	{

	}
	if (keydata.key == MLX_KEY_C && keydata.action == MLX_RELEASE)
		ctx->alt_color = !ctx->alt_color;
}

static inline void	controls(void *param)
{
	t_context	*ctx;

	ctx = param;
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
		mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_LEFT))
	{

	}
	if (mlx_is_key_down(ctx->mlx, MLX_KEY_LEFT_ALT) && \
		mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{

	}
	if (mlx_is_mouse_down(ctx->mlx, MLX_MOUSE_BUTTON_MIDDLE))
	{

	}
}
