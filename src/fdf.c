/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:19:35 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/20 19:33:01 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	loop(void *param);
static inline void	input(mlx_key_data_t keydata, void *param);

int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_context	*ctx;

	if (argc != 2)
		return (EXIT_FAILURE);
	mlx_set_setting(MLX_FULLSCREEN, true);
	mlx = mlx_init(WIDTH, HEIGHT, "FdF", true);
	if (!mlx)
		ft_mlx_error(NULL);
	img  = mlx_new_image(mlx, mlx->width, mlx->height);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_mlx_error(mlx);
	if (initialize(argv[1], &ctx, mlx, img) == ERROR)
		ft_mlx_error(mlx);
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
	static double	time_color;
	t_context		*ctx;
	double			t;

	ctx = param;
	control_camera(ctx);
	t = ctx->time_rot;
	if (ctx->colors == AMAZING)
	{
		ctx->color1 = rainbow_rgb(wrapf(time_color * 2.0f + 5.0f));
		ctx->color2 = rainbow_rgb(wrapf(time_color * 2.0f));
	}
	if (ctx->spin == ON)
	{
		ctx->transform.rot = vec3((t * 0.5f), sinf(t * 0.1f), cosf(t*0.2f) - 1);
		ctx->time_rot += ctx->mlx->delta_time;
	}
	time_color += ctx->mlx->delta_time;
	render(ctx);
}

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
	if (keydata.key == MLX_KEY_F && keydata.action == MLX_RELEASE)
		frame(ctx);
	if (keydata.key == MLX_KEY_C && keydata.action == MLX_RELEASE)
		ctx->colors = !ctx->colors;
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_RELEASE && \
		ctx->cam.projection != ISOMETRIC)
		ctx->spin = !ctx->spin;
	if (keydata.key == MLX_KEY_R && keydata.action == MLX_RELEASE)
		reset_transforms(ctx);
}



