/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:08:26 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/18 07:13:19 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline void	render_line(t_context *ctx, int i1, int i2);
void	draw_line(mlx_image_t *img, t_vertex v0, t_vertex v1, t_context *ctx);
static inline void	move_pixel(t_vec2 *d, t_vec2 *s, t_vec2 *e, t_vertex *v0);
void	draw_pixel(mlx_image_t *img, t_vertex v0, uint32_t c, t_context *ctx);

void	render(void *param)
{
	size_t		i;
	t_vec3		index;
	t_context	*ctx;

	i = 0;
	ctx = param;
	clear_image(ctx->img, BLACK);
	while (i < ctx->tris->total)
	{
		index = *(t_vec3 *)(vector_get(ctx->tris, i));
		// TODO Hash table lines to render with no dups
		render_line(ctx, index.x, index.y);
		render_line(ctx, index.y, index.z);
		//render_line(ctx, index.x, index.z);
		++i;
	}
}

static inline void	render_line(t_context *ctx, int i1, int i2)
{
	t_vertex	v0;
	t_vertex	v1;

	v0 = *(t_vertex *)(vector_get(ctx->verts, i1));
	v1 = *(t_vertex *)(vector_get(ctx->verts, i2));

	if ((mvp(&v0, ctx)) && mvp(&v1, ctx))
		draw_line(ctx->img, v0, v1, ctx);
}

void	draw_line(mlx_image_t *img, t_vertex v0, t_vertex v1, t_context *ctx)
{
	t_vec2	d;
	t_vec2	s;
	t_vec2	error;
	t_vec2	steps;
	float	t;

	d.x = abs(v1.screen.x - v0.screen.x);
	d.y = -abs(v1.screen.y - v0.screen.y);
	s.x = 1 + (-2 * (v0.screen.x >= v1.screen.x));
	s.y = 1 + (-2 * (v0.screen.y >= v1.screen.y));
	error.x = d.x + d.y;
	steps.x = 0;
	steps.y = fmaxf(d.x, -d.y);
	while (v0.screen.x != v1.screen.x || v0.screen.y != v1.screen.y)
	{
		t = (float)steps.x++ / steps.y;
		draw_pixel(img, v0, lerp_color(v0.color, v1.color, t), ctx);
		move_pixel(&d, &s, &error, &v0);
	}
	draw_pixel(img, v0, v1.color, ctx);
}

static inline void	move_pixel(t_vec2 *d, t_vec2 *s, t_vec2 *e, t_vertex *v0)
{
	e->y = 2 * e->x;
	if (e->y >= d->y)
	{
		e->x += d->y;
		v0->screen.x += s->x;
	}
	if (e->y <= d->x)
	{
		e->x += d->x;
		v0->screen.y += s->y;
	}
}

void	draw_pixel(mlx_image_t *img, t_vertex v0, uint32_t c, t_context *ctx)
{
	uint32_t color;
	//float	t;

	if (ctx->alt_color)
	{
		if (v0.pos.z <= 0)
			color = BLUE;
		else if (v0.pos.z <= ctx->max_alt / 4)
			color = GREEN;
		else if (v0.pos.z <= ctx->max_alt / 3)
			color = lerp_color(GREEN, BROWN, v0.pos.z);
		else if (v0.pos.z <= ctx->max_alt / 2)
			color = BROWN;
		else if (v0.pos.z <= (int)ctx->max_alt / 1.5f)
			color = lerp_color(BROWN, WHITE, v0.pos.z);
		else
			color = WHITE;
	}
	else
		color = c;
	if (v0.screen.x >= 0 && v0.screen.x < (int)img->width && \
		v0.screen.y >= 0 && v0.screen.y < (int)img->height)
		mlx_put_pixel(img, v0.screen.x, v0.screen.y, color);
}
