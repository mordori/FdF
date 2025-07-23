/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:24:31 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/23 21:27:19 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	if (!ctx->img || !ctx->z_buf || !mlx_resize_image(ctx->img, width, height))
	{
		fdf_free(ctx->verts, ctx->tris, ctx);
		ft_error(ctx->mlx, "resizing img failed");
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
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putendl_fd(message, STDERR_FILENO);
	if (mlx)
	{
		ft_putstr_fd("MLX42: ", STDERR_FILENO);
		perror(mlx_strerror(mlx_errno));
		mlx_terminate(mlx);
	}
	exit(EXIT_FAILURE);
}

/**
 * Frees the rendering context, Z-buffer, and the `verts` `tris` vector arrays.
 *
 * @param verts Vertices vector array.
 * @param tris Triangles vector array.
 * @param ctx Rendering context containing Z-buffer.
 */
void	fdf_free(t_vector *verts, t_vector *tris, t_context *ctx)
{
	if (verts && verts->items)
		vector_free(verts);
	if (tris && tris->items)
		vector_free(tris);
	free(verts);
	free(tris);
	if (ctx)
		free(ctx->z_buf);
	free(ctx);
}
