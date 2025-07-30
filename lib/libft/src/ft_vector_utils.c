/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:13:36 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/24 22:08:56 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_vector.h"

/**
 * Creates a new items array and replaces the old one.
 *
 * @param vec Vector to be operated.
 * @param size Capacity of the new items array to be allocated.
 * @return True if successful, else false.
 */
static inline bool	vector_resize(t_vector *vec, size_t size)
{
	void	**items;
	size_t	i;

	items = malloc(sizeof (void *) * size);
	if (!items)
		return (false);
	i = 0;
	while (i < vec->total)
	{
		items[i] = vec->items[i];
		++i;
	}
	free(vec->items);
	vec->items = items;
	vec->size = size;
	return (true);
}

/**
 * Adds `new` to the items of `vec`.
 *
 * Resizes the items array if needed.
 *
 * @param vec Vector to be operated.
 * @param new New item to be added.
 * @return True if successful, else false.
 */
bool	vector_add(t_vector *vec, void *new)
{
	if (!vec || !vec->size)
		return (false);
	if (vec->total == vec->size)
		if (!vector_resize(vec, vec->size * 2))
			return (false);
	vec->items[vec->total++] = new;
	return (true);
}

/**
 * Removes an item with the `index` in `vec`.
 *
 * Resizes the items array if needed.
 *
 * Frees the item, if it belongs to heap.
 *
 * @param vec Vector to be operated.
 * @param index Index of the item to be removed.
 * @return True if successful, else false.
 */
bool	vector_del(t_vector *vec, size_t index)
{
	const size_t	min_size = 4;

	if (!vec || !vec->total || index >= vec->total)
		return (false);
	if (vec->is_heap)
	{
		free(vec->items[index]);
		vec->items[index] = NULL;
	}
	while (index < vec->total - 1)
	{
		vec->items[index] = vec->items[index + 1];
		++index;
	}
	vec->total--;
	if (vec->size > min_size && vec->total > 0 && \
vec->total == vec->size / 4)
		if (!vector_resize(vec, vec->size / 2))
			return (false);
	return (true);
}

/**
 * Frees the items array and cleans ´vec´.
 *
 * Additionally frees all item elements, if they belong to heap.
 *
 * @param vec Vector to be operated.
 * @return True if successful, else false.
 */
bool	vector_free(t_vector *vec)
{
	size_t	i;

	if (!vec || !vec->size)
		return (false);
	i = 0;
	if (vec->total && vec->is_heap)
	{
		while (i < vec->total)
		{
			free(vec->items[i]);
			vec->items[i++] = NULL;
		}
	}
	free(vec->items);
	vec->total = 0;
	vec->size = 0;
	vec->items = NULL;
	return (true);
}

/**
 * Returns the last item in `vec`.
 *
 * @param vec Vector to be operated.
 * @return Item if successful, else NULL.
 */
void	*vector_getlast(t_vector *vec)
{
	if (!vec || !vec->total)
		return (NULL);
	return (vec->items[vec->total - 1]);
}
