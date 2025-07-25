/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 02:40:17 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 15:08:31 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static bool	clip_test(float p, float q, float *t0, float *t1);

bool liang_barsky(t_vec4 *v0, t_vec4 *v1)
{
	float	t0;
	float	t1;
	t_vec4	d;

	t0 = 0.0f;
	t1 = 1.0f;
	d = vec4(v1->x - v0->x, v1->y - v0->y, v1->z - v0->z, v1->w - v0->w);
	if (!clip_test(-d.x - d.w, v0->x + v0->w, &t0, &t1)) //LEFT
		return (false);
	if (!clip_test(d.x - d.w, -v0->x + v0->w, &t0, &t1)) //RIGHT
		return (false);
	if (!clip_test(-d.y - d.w, v0->y + v0->w, &t0, &t1)) //BOTTOM
		return (false);
	if (!clip_test(d.y - d.w, -v0->y + v0->w, &t0, &t1)) //TOP
		return (false);
	if (!clip_test(-d.z - d.w, v0->z + v0->w, &t0, &t1)) //NEAR
		return (false);
	if (!clip_test(d.z - d.w, -v0->z + v0->w, &t0, &t1)) //FAR
		return (false);
	if (t1 < 1.0f)
		*v1 = vec4(v0->x + t1 * d.x, v0->y + t1 * d.y,
				v0->z + t1 * d.z, v0->w + t1 * d.w);
	if (t0 > 0.0f)
		*v0 = vec4_add(*v0, vec4(t0 * d.x, t0 * d.y, t0 * d.z, t0 * d.w));
	return (true);
}

static bool	clip_test(float p, float q, float *t0, float *t1)
{
	float	t;

	if (fabsf(p) < 1e-6f)
		return (q >= 0);
	t = q / p;
	if (p < 0)
	{
		if (t > *t1)
			return (false);
		if (t > *t0)
			*t0 = t;
	}
	else if (p > 0)
	{
		if (t < *t0)
			return (false);
		if (t < *t1)
			*t1 = t;
	}
	return (true);
}
