/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 02:40:17 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 19:16:50 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline bool	clip_test(float p, float q, float *t0, float *t1);

bool liang_barsky_clip(t_vertex *v0, t_vertex *v1)
{
	float	t0;
	float	t1;
	t_vec4	d;

	t0 = 0.0f;
	t1 = 1.0f;
	d = vec4_sub(v1->pos, v0->pos);
	if (!clip_test(-d.x - d.w, v0->pos.x + v0->pos.w, &t0, &t1)) //LEFT
		return (false);
	if (!clip_test(d.x - d.w, -v0->pos.x + v0->pos.w, &t0, &t1)) //RIGHT
		return (false);
	if (!clip_test(-d.y - d.w, v0->pos.y + v0->pos.w, &t0, &t1)) //BOTTOM
		return (false);
	if (!clip_test(d.y - d.w, -v0->pos.y + v0->pos.w, &t0, &t1)) //TOP
		return (false);
	if (!clip_test(-d.z - d.w, v0->pos.z + v0->pos.w, &t0, &t1)) //NEAR
		return (false);
	if (!clip_test(d.z - d.w, -v0->pos.z + v0->pos.w, &t0, &t1)) //FAR
		return (false);
	if (t1 < 1.0f)
	{
		v1->pos = vec4_add(v0->pos, vec4_scale(d, t1));
		v1->color = lerp_color(v0->color, v1->color, t1);
	}
	if (t0 > 0.0f)
	{
		v0->pos = vec4_add(v0->pos, vec4_scale(d, t0));
		v0->color = lerp_color(v0->color, v1->color, t0);
	}
	return (true);
}

bool liang_barsky_screen(t_context *ctx, t_vec4 *v)
{
	float	t0;
	float	t1;
	t_vec2	d;

	t0 = 0.0f;
	t1 = 1.0f;
	d = vec2(v->z - v->x, v->w - v->y);
	if (!clip_test(-d.x, v->x, &t0, &t1))						//LEFT
		return (false);
	if (!clip_test(d.x, ctx->img->width - 1 - v->x, &t0, &t1))	//RIGHT
		return (false);
	if (!clip_test(-d.y, v->y, &t0, &t1))						//BOTTOM
		return (false);
	if (!clip_test(d.y, ctx->img->height - 1 - v->y, &t0, &t1))	//TOP
		return (false);
	if (t1 < 1.0f)
	{
		v->z = v->x + t1 * d.x;
		v->w = v->y + t1 * d.y;
	}
	if (t0 > 0.0f)
	{
		v->x += t0 * d.x;
		v->y += t0 * d.y;
	}
	return (true);
}

static inline bool	clip_test(float p, float q, float *t0, float *t1)
{
	float	t;

	if (p == 0.0f)
		return (q >= 0.0f);
	t = q / p;
	if (p < 0.0f)
	{
		if (t > *t1)
			return (false);
		if (t > *t0)
			*t0 = t;
	}
	else
	{
		if (t < *t0)
			return (false);
		if (t < *t1)
			*t1 = t;
	}
	return (true);
}
