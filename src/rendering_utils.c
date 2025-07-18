/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:09:05 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/19 07:22:21 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	clear_image(mlx_image_t *img, uint32_t color)
{
	size_t		i;
	size_t		width;
	size_t		height;
	uint32_t	*pixels;

	pixels = (uint32_t *)img->pixels;
	width = img->width;
	height = img->height;
	i = 0;
	while (i < width)
		pixels[i++] = color;
	i = 1;
	while (i < height)
		ft_memcpy(&pixels[i++ * width], pixels, width * sizeof (uint32_t ));
}

uint32_t	lerp_color(uint32_t c1, uint32_t c2, float t)
{
	t_color color1;
	t_color color2;
	t_color color;

	color1.r = (c1 >> 24) & 0xFF;
	color1.g = (c1 >> 16) & 0xFF;
	color1.b = (c1 >> 8) & 0xFF;
	color1.a = c1 & 0xFF;
	color2.r = (c2 >> 24) & 0xFF;
	color2.g = (c2 >> 16) & 0xFF;
	color2.b = (c2 >> 8) & 0xFF;
	color2.a = c2 & 0xFF;
	color.r = (uint8_t)ft_lerp(color1.r, color2.r, t);
	color.g = (uint8_t)ft_lerp(color1.g, color2.g, t);
	color.b = (uint8_t)ft_lerp(color1.b, color2.b, t);
	color.a = (uint8_t)ft_lerp(color1.a, color2.a, t);
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
}

void	update_ui(t_context *ctx)
{
	static mlx_image_t	*proj;
	static mlx_image_t	*info;
	static mlx_image_t	*controls;
	char *str;

	if (proj)
		mlx_delete_image(ctx->mlx, proj);
	if (info)
		mlx_delete_image(ctx->mlx, info);
	if (controls)
		mlx_delete_image(ctx->mlx, controls);
	str = "[ALT]+[MMB]pan   [ALT]+[RMB]zoom";
	if (ctx->cam.projection == ISOMETRIC)
		proj = mlx_put_string(ctx->mlx, "Isometric", 100, 60);
	else
		str = "[ALT]+[MMB]pan   [ALT]+[RMB]zoom   [ALT]+[LMB]orbit";
	if (ctx->cam.projection == ORTHOGRAPHIC)
		proj = mlx_put_string(ctx->mlx, "Orthographic", 100, 60);
	if (ctx->cam.projection == PERSPECTIVE)
		proj = mlx_put_string(ctx->mlx, "Perspective", 100, 60);
	controls = mlx_put_string(ctx->mlx, str, 100, ctx->mlx->height - 75);
	str = "[ESC]quit   [P]projection   [F]focus";
	info = mlx_put_string(ctx->mlx, str, 100, ctx->mlx->height - 110);
	info->instances[0].z = 101;
	controls->instances[0].z = 102;
}

void	update_ui_2(t_context *ctx)
{
	static mlx_image_t	*info;
	char *str;

	if (info)
		mlx_delete_image(ctx->mlx, info);
	if (ctx->cam.projection == ISOMETRIC)
		str = "[WASD]translate";
	else
		str = "[WASD]translate   [R]rotate";
	info = mlx_put_string(ctx->mlx, str, 100, ctx->mlx->height - 145);
	info->instances[0].z = 103;
}
