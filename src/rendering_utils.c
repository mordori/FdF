/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:09:05 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/18 01:55:17 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	clear_image(mlx_image_t *img, uint32_t color)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (i < img->width)
	{
		j = 0;
		while (j < img->height)
		{
			mlx_put_pixel(img, i, j, color);
			++j;
		}
		++i;
	}
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
