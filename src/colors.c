/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:09:05 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/29 19:31:30 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Clears the render image pixels with a solid color and the Z-buffer to INF.
 * Sets the data for the first row, then copies it to the subsequent rows.
 *
 * @param ctx Model context containing render image and Z-buffer.
 * @param color Color (32-bit RGBA).
 */
void	clear_image(t_context *ctx, uint32_t color)
{
	size_t		i;
	size_t		width;
	size_t		height;
	uint32_t	*pixels;

	pixels = (uint32_t *)ctx->img->pixels;
	width = ctx->img->width;
	height = ctx->img->height;
	i = 0;
	while (i < width)
	{
		ctx->z_buf[i] = INFINITY;
		pixels[i] = color;
		++i;
	}
	i = 1;
	while (i < height)
	{
		ft_memcpy(&pixels[i * width], pixels, width * sizeof (uint32_t));
		ft_memcpy(&ctx->z_buf[i * width], ctx->z_buf, width * sizeof (float));
		++i;
	}
}

uint32_t	rgba_to_abgr(uint32_t rgba)
{
	t_color	abgr;

	abgr.a = (rgba & 0xFF) << 24;
	abgr.b = (rgba & 0xFF00) << 8;
	abgr.g = (rgba & 0xFF0000) >> 8;
	abgr.r = (rgba & 0xFF000000) >> 24;
	return (abgr.a | abgr.b | abgr.g | abgr.r);
}

/**
 * Interpolates two colors by first separating the color channels,
 * then interpolates them, and finally combines everything back into a 32-bit
 * RGBA color.
 *
 * @param c1 Starting color (32-bit RBGA).
 * @param c2 Target color (32-bit RBGA).
 * @param t Interpolation factor.
 * @return Interpolated color (32-bit RGBA).
 */
uint32_t	lerp_color(uint32_t c1, uint32_t c2, float t)
{
	t_color	color1;
	t_color	color2;
	t_color	color;

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

/**
 * Generates a time-based rainbow color. Uses offset sine waves to oscillate
 * RGB channels between [0 - 255].
 *
 * @param t Time, animates the sine waves.
 * @return New color (32-bit RGBA).
 */
uint32_t	rainbow_rgb(double t)
{
	float	r;
	float	g;
	float	b;

	r = (sin(t) * 0.5f + 0.5f) * 255.0f;
	g = (sin(t + 2.0f) * 0.5f + 0.5f) * 255.0f;
	b = (sin(t + 4.0f) * 0.5f + 0.5f) * 255.0f;
	return ((uint8_t)r << 24 | (uint8_t)g << 16 | (uint8_t)b << 8 | 0xFF);
}
