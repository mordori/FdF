/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 01:10:00 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/30 22:35:09 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	update_ui_1(t_context *ctx);
static inline void	update_ui_2(t_context *ctx);
static inline void	update_ui_3(t_context *ctx);

void	update_ui(t_context *ctx)
{
	update_ui_1(ctx);
	update_ui_2(ctx);
	update_ui_3(ctx);
}

/**
 * Updates the main UI elements (projection mode and control hints).
 *
 * @param ctx Render context containing camera and mlx context.
 */
static inline void	update_ui_1(t_context *ctx)
{
	static mlx_image_t	*proj;

	if (proj)
		mlx_delete_image(ctx->mlx, proj);
	if (ctx->cam.projection == ISOMETRIC)
		proj = mlx_put_string(ctx->mlx, "Isometric", 100, 60);
	else if (ctx->cam.projection == ORTHOGRAPHIC)
		proj = mlx_put_string(ctx->mlx, "Orthographic", 100, 60);
	else if (ctx->cam.projection == PERSPECTIVE)
		proj = mlx_put_string(ctx->mlx, "Perspective", 100, 60);
	if (!proj)
		fdf_free(ctx->verts, ctx->tris, ctx), \
ft_error(ctx->mlx, "ui 1", ctx);
}

/**
 * Updates additional UI control hints.
 *
 * @param ctx Render context containing camera and mlx context.
 */
static inline void	update_ui_2(t_context *ctx)
{
	static mlx_image_t	*controls;
	char				*str;
	int					y;

	if (controls)
		mlx_delete_image(ctx->mlx, controls);
	if (ctx->cam.projection == PERSPECTIVE)
		str = "[WASD]move  [ARROWS]rotate  [SPACE]spin  [F]frame  [U-I]fov";
	else
		str = "[WASD]move  [ARROWS]rotate  [SPACE]spin  [F]frame";
	if (ctx->cam.projection != ISOMETRIC)
	{
		y = ft_imax(100, ctx->img->height - 110);
		controls = mlx_put_string(ctx->mlx, str, 100, y);
		if (!controls)
			fdf_free(ctx->verts, ctx->tris, ctx), \
ft_error(ctx->mlx, "ui 2", ctx);
		controls->instances[0].z = 101;
	}
}

static inline void	update_ui_3(t_context *ctx)
{
	static mlx_image_t	*info;
	static mlx_image_t	*controls;
	char				*str_i;
	char				*str_c;

	if (info)
		mlx_delete_image(ctx->mlx, info);
	if (controls)
		mlx_delete_image(ctx->mlx, controls);
	str_i = "[ESC]quit  [P]projection [C]color  [R]reset";
	if (ctx->cam.projection == ISOMETRIC)
		str_i = "[ESC]quit  [P]projection";
	info = mlx_put_string(ctx->mlx, str_i, 100,
			ft_imax(100, ctx->img->height - 75));
	if (!info)
		fdf_free(ctx->verts, ctx->tris, ctx), \
ft_error(ctx->mlx, "ui 3-1", ctx);
	info->instances[0].z = 102;
	if (ctx->cam.projection == ISOMETRIC)
		return ;
	str_c = "[MMB]pan  [RMB]zoom  [LMB]orbit";
	controls = mlx_put_string(ctx->mlx, str_c, 100,
			ft_imax(100, ctx->img->height - 145));
	if (!controls)
		fdf_free(ctx->verts, ctx->tris, ctx), \
ft_error(ctx->mlx, "ui 3-2", ctx);
	controls->instances[0].z = 103;
}
