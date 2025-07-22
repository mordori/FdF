/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:19:35 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/22 19:10:36 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "fdf_2.h"

static inline void	loop(void *param);
static inline void	input(mlx_key_data_t keydata, void *param);

/**
 * Simple wireframe model software renderer with limited file parsing.
 *
 * Known flaws:
 *
 * - Flipped projection with some axis rotations.
 *
 * - Clipping is implemented only partially.
 *
 * - Lines on the last row and column are rendered twice.
 *
 * - Final pixel color on a line off by 1 stop from v1.
 *
 * - Inadequate performance with higher vertex amounts,
 * probably due to the use of too many instructions and dynamic vector arrays.
 *
 * Main initializes mlx context, render imgage, sets the loop hooks,
 * and begins model initialization.
 *
 * @param argc Arguments count.
 * @param argv File path e.g. "maps/42.fdf"
 */
int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_context	*ctx;

	if (argc != 2)
		return (ft_error(NULL, "arguments"), EXIT_FAILURE);
	mlx_set_setting(MLX_MAXIMIZED, true);
	mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
		ft_error(NULL, "mlx alloc");
	img = mlx_new_image(mlx, mlx->width, mlx->height);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error(mlx, "img alloc");
	initialize(argv[1], &ctx, mlx, img);
	mlx_loop_hook(mlx, loop, ctx);
	mlx_key_hook(mlx, input, ctx);
	mlx_resize_hook(mlx, resize, ctx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	fdf_free(ctx->verts, ctx->tris, ctx);
	return (EXIT_SUCCESS);
}

/**
 * Main loop for camera update and rendering.
 *
 * @param param Model context.
 */
static inline void	loop(void *param)
{
	static double	time_color;
	t_context		*ctx;
	double			t;

	ctx = param;
	if (!ctx->img)
		return ;
	control_camera(ctx);
	t = ctx->time_rot;
	if (ctx->colors == AMAZING)
	{
		ctx->color1 = rainbow_rgb(wrapf(-time_color * 2.0f + M_PI_2));
		ctx->color2 = rainbow_rgb(wrapf(-time_color * 2.0f));
	}
	if (ctx->spin == ON)
	{
		ctx->transform.rot.x = t * 0.5f;
		ctx->transform.rot.y = sinf(t * 0.1f);
		ctx->transform.rot.z = cosf(t * 0.2f) - 1;
		ctx->time_rot += ctx->mlx->delta_time;
	}
	time_color += ctx->mlx->delta_time;
	render(ctx);
}

/**
 * Loop hook for discrete input commands.
 *
 * @param keydata Mlx key data.
 * @param param Model context.
 */
static inline void	input(mlx_key_data_t keydata, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_RELEASE)
		mlx_close_window(ctx->mlx);
	if (keydata.key == MLX_KEY_P && keydata.action == MLX_RELEASE)
	{
		ctx->cam.projection = (ctx->cam.projection + 1) % 3;
		if (ctx->cam.projection == ISOMETRIC)
			reset_transforms(ctx);
	}
	if (keydata.key == MLX_KEY_C && keydata.action == MLX_RELEASE)
		ctx->colors = !ctx->colors;
	if (ctx->cam.projection != ISOMETRIC)
	{
		if (keydata.key == MLX_KEY_F && keydata.action == MLX_RELEASE && \
ctx->spin == OFF)
			frame(ctx);
		if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_RELEASE)
			ctx->spin = !ctx->spin;
		if (keydata.key == MLX_KEY_R && keydata.action == MLX_RELEASE)
			reset_transforms(ctx);
	}
}
