/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:14:56 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/18 05:06:53 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_vertex	*make_vert(float x, float y, float z, uint32_t color)
{
	t_vertex *vertex;

	vertex = malloc(sizeof (t_vertex));
	if (!vertex)
		return (NULL);
	vertex->pos.x = x;
	vertex->pos.y = y;
	vertex->pos.z = z;
	vertex->pos.w = 1.0f;
	vertex->color = color;
	return (vertex);
}

void	make_triangles(t_vector *tris, t_vec2 rows_cols)
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
		vector_add(tris, tri);
		++i;
	}
}

t_vec3	*make_tri(int x, int y, int z)
{
	t_vec3 *vec;

	vec = malloc(sizeof (t_vec3));
	if (!vec)
		return (NULL);
	vec->x = x;
	vec->y = y;
	vec->z = z;
	return (vec);
}
