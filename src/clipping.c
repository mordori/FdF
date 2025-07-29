/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 02:40:17 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/29 14:39:52 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static inline bool	clip_test(float p, float q, float *t0, float *t1);

/**
 * Performs Liang-Barsky clipping in homogeneous clip space (3D).
 * The function clips geometry outside the camera’s viewing frustum
 * defined by near, far, left, right, top, and bottom planes (-w <= x,y,z <= w).
 *
 * If the line segment lies partially outside the view volume, the function
 * modifies `v0` and/or `v1` to clip them. Colors are interpolated at clipped
 * points for smooth shading.
 *
 * @param v0 First vertex of the line.
 * @param v1 Second vertex of the line.
 * @return True if the clipped segment is visible.
 */
bool liang_barsky_clip(t_vertex *v0, t_vertex *v1)
{
	t_vec2	t;
	t_vec4	d;

	t = vec2(0.0f, 1.0f);
	d = vec4_sub(v1->pos, v0->pos);
	if ((!clip_test(-d.x - d.w, v0->pos.x + v0->pos.w, &t.x, &t.y)) || \
(!clip_test(d.x - d.w, -v0->pos.x + v0->pos.w, &t.x, &t.y)) || \
(!clip_test(-d.y - d.w, v0->pos.y + v0->pos.w, &t.x, &t.y)) || \
(!clip_test(d.y - d.w, -v0->pos.y + v0->pos.w, &t.x, &t.y)) || \
(!clip_test(-d.z - d.w, v0->pos.z + v0->pos.w, &t.x, &t.y)) || \
(!clip_test(d.z - d.w, -v0->pos.z + v0->pos.w, &t.x, &t.y)))
		return (false);
	if (t.y < 1.0f)
	{
		v1->pos = vec4_add(v0->pos, vec4_scale(d, t.y));
		v1->color = lerp_color(v0->color, v1->color, t.y);
	}
	if (t.x > 0.0f)
	{
		v0->pos = vec4_add(v0->pos, vec4_scale(d, t.x));
		v0->color = lerp_color(v0->color, v1->color, t.x);
	}
	return (true);
}

/**
 * Clips a line segment against the screen boundaries.
 * This is a 2D variant of the Liang-Barsky algorithm. It operates on a
 * rectangle defined by the screen's pixel boundaries (0, width/height).
 *
 * @param ctx Render context containing screen dimensions.
 * @param v0 First vertex of the line.
 * @param v1 Second vertex of the line.
 * @return True if the clipped segment is visible within the screen.
 */
bool liang_barsky_screen(t_context *ctx, t_vertex *v0, t_vertex *v1)
{
	t_vec2	t;
	t_vec2	d;

	t = vec2(0.0f, 1.0f);
	d = vec2(v1->s.x - v0->s.x, v1->s.y - v0->s.y);
	if ((!clip_test(-d.x, v0->s.x, &t.x, &t.y)) || \
(!clip_test(d.x, ctx->img->width - 1 - v0->s.x, &t.x, &t.y)) || \
(!clip_test(-d.y, v0->s.y, &t.x, &t.y)) || \
(!clip_test(d.y, ctx->img->height - 1 - v0->s.y, &t.x, &t.y)))
		return (false);
	if (t.y < 1.0f)
	{
		v1->s.x = v0->s.x + t.y * d.x;
		v1->s.y = v0->s.y + t.y * d.y;
	}
	if (t.x > 0.0f)
	{
		v0->s.x += t.x * d.x;
		v0->s.y += t.x * d.y;
	}
	return (true);
}

/**
 * Determines whether a segment lies within the clipping bounds for a single
 * boundary. Adjusts the `t0` and `t1` parameters, which represent the
 * parametric range of the visible segment (0 <= t <= 1).
 *
 * @param p Directional component of the line relative to the clipping boundary.
 * @param q Distance from the line's starting point to the clipping boundary.
 * @param t0 Minimum visible parameter of the line segment.
 * @param t1 Maximum visible parameter of the line segment.
 * @return True if the segment is at least partially within the boundary.
 */
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
