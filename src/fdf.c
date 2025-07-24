/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:19:35 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/24 19:11:31 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	loop(void *param);

/**
 * Simple wireframe model software renderer with limited file parsing.
 *
 * Follows an OpenGL-style right-handed coordinate convention,
 * where Y is up, X is to the right, and in view space the camera looks down -Z.
 *
 * Known flaws:
 *
 * - Clipping is implemented only partially.
 *
 * - Lines on the last row and column of triangles are rendered twice.
 *
 * - Inadequate performance with higher vertex amounts, there is much
 * optimization that could be done.
 *
 * - Panning could to be more accurate.
 *
 * - Orthographic projections easily introduce an optical illusion where the
 * model seemingly flips at certain camera angles.
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
	if (!img || mlx_image_to_window(mlx, img, 0, 0) == ERROR)
		ft_error(mlx, "img alloc");
	initialize(argv[1], &ctx, mlx, img);
	mlx_loop_hook(mlx, loop, ctx);
	mlx_key_hook(mlx, key_hook, ctx);
	mlx_resize_hook(mlx, resize, ctx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	fdf_free(ctx->verts, ctx->tris, ctx);
	return (EXIT_SUCCESS);
}

/**
 * Main loop for camera update, rendering, and ui update.
 * Iterates the values used for spin and color features.
 *
 * @param param Rendering context.
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
		ctx->color1 = rainbow_rgb(-time_color * 2.0f + M_PI_2);
		ctx->color2 = rainbow_rgb(-time_color * 2.0f);
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
	update_ui(ctx);
	update_ui_2(ctx);
}
