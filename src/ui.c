/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 01:10:00 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 01:10:38 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Updates the main UI elements (projection mode and control hints).
 *
 * @param ctx Render context containing camera and mlx context.
 */
void	update_ui(t_context *ctx)
{
	static mlx_image_t	*proj;
	static mlx_image_t	*info;
	static mlx_image_t	*controls;
	char				*str;

	if (proj)
		mlx_delete_image(ctx->mlx, proj);
	if (info)
		mlx_delete_image(ctx->mlx, info);
	if (controls)
		mlx_delete_image(ctx->mlx, controls);
	if (ctx->cam.projection == ISOMETRIC)
		proj = mlx_put_string(ctx->mlx, "Isometric", 100, 60);
	if (ctx->cam.projection == ORTHOGRAPHIC)
		proj = mlx_put_string(ctx->mlx, "Orthographic", 100, 60);
	if (ctx->cam.projection == PERSPECTIVE)
		proj = mlx_put_string(ctx->mlx, "Perspective", 100, 60);
	str = "[ESC]quit  [C]color  [P]projection";
	info = mlx_put_string(ctx->mlx, str, 100, ctx->img->height - 75);
	info->instances[0].z = 101;
	str = "[ALT]+[MMB]pan   [ALT]+[RMB]zoom   [ALT]+[LMB]orbit";
	if (ctx->cam.projection == ISOMETRIC)
		str = " ";
	controls = mlx_put_string(ctx->mlx, str, 100, ctx->img->height - 110);
	controls->instances[0].z = 102;
}

/**
 * Updates additional UI control hints.
 *
 * @param ctx Render context containing camera and mlx context.
 */
void	update_ui_2(t_context *ctx)
{
	static mlx_image_t	*info;
	char				*str;

	if (info)
		mlx_delete_image(ctx->mlx, info);
	str = "[WASD]translate  [ARROWS]rotate  [SPACE]spin  [F]frame  [R]reset";
	if (ctx->cam.projection == ISOMETRIC)
		str = " ";
	info = mlx_put_string(ctx->mlx, str, 100, ctx->img->height - 145);
	info->instances[0].z = 103;
}
