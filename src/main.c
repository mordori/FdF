/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 17:19:35 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/29 19:47:25 by myli-pen         ###   ########.fr       */
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
 * Orthographic projections easily introduce an optical illusion where the
 * model seemingly flips at certain camera angles.
 *
 * Panning still needs to be improved.
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
	fdf_free(ctx->verts, ctx->tris, ctx, NULL);
	return (EXIT_SUCCESS);
}

/**
 * Main loop for camera, rendering, and ui.
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
	update_camera(&ctx->cam);
	t = ctx->time_rot;
	if (ctx->color_mode == AMAZING)
	{
		ctx->color1 = rainbow_rgb(-time_color * 2.0f + M_PI_2);
		ctx->color2 = rainbow_rgb(-time_color * 2.0f);
	}
	if (ctx->spin_mode == ON)
	{
		ctx->transform.rot.x = t * 0.5f;
		ctx->transform.rot.y = sinf(t * 0.1f);
		ctx->transform.rot.z = cosf(t * 0.2f) - 1;
		ctx->time_rot += ctx->mlx->delta_time;
	}
	time_color += ctx->mlx->delta_time;
	render(ctx);
	update_ui(ctx);
}

/**
 * Resize hook, sets the render image to new dimensions.
 * Frames the model with the new aspect.
 *
 * @param width New window width.
 * @param height New window height.
 * @param param Rendering context containing render image and Z-buffer.
 */
void	resize(int width, int height, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (!ctx || !ctx->mlx || !ctx->img || width == 0 || height == 0)
		return ;
	free(ctx->z_buf);
	ctx->z_buf = malloc(sizeof (float) * width * height);
	if (!ctx->z_buf || !mlx_resize_image(ctx->img, width, height))
	{
		fdf_free(ctx->verts, ctx->tris, ctx, "resizing failed");
	}
	frame(ctx);
}

/**
 * Logs errors on stderr and terminates mlx before exiting.
 *
 * @param mlx Mlx context.
 * @param message Error message.
 */
void	ft_error(mlx_t *mlx, char *message)
{
	ft_putstr_fd("FdF:\tError: ", STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
	if (mlx)
	{
		ft_putstr_fd("MLX42:\t", STDERR_FILENO);
		perror(mlx_strerror(mlx_errno));
		mlx_terminate(mlx);
	}
	exit(EXIT_FAILURE);
}

/**
 * Frees the rendering context, Z-buffer, and the `verts` `tris` vector arrays.
 * Should not be called with a vector that has not called vector_init()!
 * When a message is provided it means an error has occurred.
 *
 * @param verts Vertices vector array.
 * @param tris Triangles vector array.
 * @param ctx Rendering context containing Z-buffer.
 * @param message Error message.
 */
void	fdf_free(t_vector *verts, t_vector *tris, t_context *ctx, char *msg)
{
	mlx_t	*mlx;

	mlx = ctx->mlx;
	if (verts)
		vector_free(verts);
	if (tris)
		vector_free(tris);
	free(verts);
	free(tris);
	if (ctx)
		free(ctx->z_buf);
	free(ctx);
	if (msg)
		ft_error(mlx, msg);
}
