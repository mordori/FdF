/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:14:56 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/18 22:49:23 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_vertex	*make_vert(float x, float y, float z, uint32_t color)
{
	t_vertex *v;

	v = malloc(sizeof (t_vertex));
	if (!v)
		return (NULL);
	v->pos = vec4(x, -y, z, 1.0f);
	v->color = color;
	return (v);
}

bool	make_triangles(t_vector *tris, t_vec2 rows_cols)
{
	t_vec3	*tri;
	t_quad	q;
	int row;
	int col;
	size_t	i;

	i = 0;
	while (i < (size_t)(rows_cols.x - 1) * ((rows_cols.y - 1) * 2))
	{
		row = i / (2 * (rows_cols.y - 1));
		col = (i / 2) % (size_t)(rows_cols.y - 1);
		q.topLeft = row * rows_cols.y + col;
		q.topRight = q.topLeft + 1;
		q.bottomLeft = q.topLeft + rows_cols.y;
		q.bottomRight = q.bottomLeft + 1;
		if (i % 2 == 0)
			tri = make_tri(q.topRight, q.topLeft, q.bottomLeft);
		else
			tri = make_tri(q.bottomLeft, q.bottomRight, q.topRight);
		if (!tri)
			return (false);
		vector_add(tris, tri);
		++i;
	}
	return (true);
}

t_vec3	*make_tri(int x, int y, int z)
{
	t_vec3 *vec;

	vec = malloc(sizeof (t_vec3));
	if (!vec)
		return (NULL);
	*vec = vec3(x, y, z);
	return (vec);
}
