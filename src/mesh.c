/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 23:14:56 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/25 19:46:57 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/**
 * Creates a dynamically allocated vertex from given position and color.
 * Initializes its position, color, screen coordinates, and depth.
 *
 * @param x X position in object space.
 * @param y Y position in object space (negated).
 * @param z Z position in object space.
 * @param color Vertex color (32-bit RGBA).
 * @return Pointer to a new vertex.
 */
t_vertex	*make_vert(float x, float y, float z, uint32_t color)
{
	t_vertex	*v;

	v = malloc(sizeof (t_vertex));
	if (!v)
		return (NULL);
	v->pos = vec4(x, -y, z, 1.0f);
	v->color = color;
	v->s = vec2i(0, 0);
	v->depth = 0.0f;
	return (v);
}

/**
 * Divides a grid of vertices into triangles and stores the triangle vertex
 * indices as `vec3` structures inside the given `tris` vector.
 * Each quad formed by adjacent vertices is split into two triangles.
 *
 * @param tris Vector containing the generated triangles.
 * @param rows_cols A tuple containing number of vertex rows(x) and columns(y).
 * @return `true` on success, `false` if memory allocation fails.
 */
bool	make_triangles(t_vector *tris, t_vec2i rows_cols)
{
	t_vec3	*tri;
	t_quad	q;
	int		row;
	int		col;
	size_t	i;

	i = 0;
	while (i < (size_t)(rows_cols.x - 1) * ((rows_cols.y - 1) * 2))
	{
		row = i / (2 * (rows_cols.y - 1));
		col = (i / 2) % (size_t)(rows_cols.y - 1);
		q.topleft = row * rows_cols.y + col;
		q.topright = q.topleft + 1;
		q.bottomleft = q.topleft + rows_cols.y;
		q.bottomright = q.bottomleft + 1;
		if (i++ % 2 == 0)
			tri = make_tri(q.topright, q.topleft, q.bottomleft);
		else
			tri = make_tri(q.bottomleft, q.bottomright, q.topright);
		if (!tri)
			return (false);
		if (!vector_add(tris, tri))
			return (false);
	}
	return (true);
}

/**
 * Creates a dynamically allocated triangle from given vertex indices.
 *
 * @param x Index of the first vertex.
 * @param y Index of the second vertex
 * @param z Index of the third vertex
 * @return Pointer to a new triangle.
 */
t_vec3	*make_tri(int x, int y, int z)
{
	t_vec3	*vec;

	vec = malloc(sizeof (t_vec3));
	if (!vec)
		return (NULL);
	*vec = vec3(x, y, z);
	return (vec);
}
