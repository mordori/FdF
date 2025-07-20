/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:24:31 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/19 19:22:14 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	on_close(void* param)
{
	t_context	*ctx;

	ctx = param;
	(void)ctx;
	printf("CLOSINGGGG\n");
}

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
		ft_mlx_error(ctx->mlx);
	}
	ctx->cam.aspect = (float)ctx->mlx->width / ctx->mlx->height;
	frame(ctx);
}

void ft_mlx_error(mlx_t *mlx)
{
	ssize_t	bytes;

	if (mlx)
	{
		bytes = write(STDERR_FILENO, "MLX42 - ", 8);
		(void)bytes;
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
	free(ctx);
}
