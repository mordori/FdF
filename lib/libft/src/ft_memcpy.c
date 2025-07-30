/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:11:53 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/03 01:41:00 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_mem.h"

/**
 * Copies `n` bytes from `src` to `dest`.
 *
 * The memory areas must not overlap.
 *
 * @param dest Destination memory area.
 * @param src Source memory area.
 * @param n Number of bytes to be written.
 * @return Pointer to modified `dest`.
 */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	if ((!dest || !src) && n)
		return (NULL);
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	while (n--)
		*d++ = *s++;
	return (dest);
}
