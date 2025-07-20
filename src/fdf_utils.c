/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:24:31 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/21 00:34:30 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	on_resize(int width, int height, void *param)
{
	t_context	*ctx;

	ctx = param;
	if (ctx->img)
		mlx_delete_image(ctx->mlx, ctx->img);
	ctx->img = mlx_new_image(ctx->mlx, width, height);
	if (!ctx->img || (mlx_image_to_window(ctx->mlx, ctx->img, 0, 0) < 0))
	{
		if (ctx && ctx->verts && ctx->tris)
			fdf_free(ctx->verts, ctx->tris, ctx);
		ft_error(ctx->mlx, "mlx: resize image failed");
	}
	ctx->cam.aspect = (float)ctx->mlx->width / ctx->mlx->height;
	frame(ctx);
}

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

void	fdf_free(t_vector *verts, t_vector *tris, t_context *ctx)
{
	if (verts && verts->items)
		vector_free(verts);
	if (tris && tris->items)
		vector_free(tris);
	free(verts);
	free(tris);
	free(ctx->z_buf);
	free(ctx);
}
