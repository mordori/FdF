/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:58:00 by myli-pen          #+#    #+#             */
/*   Updated: 2025/07/02 18:19:14 by myli-pen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_str.h"

/**
 * Calculates the amount of characters in string `s`.
 *
 * @param s Source string.
 * @return The length of the string `s`.
 */
size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (!s)
		return (false);
	len = 0;
	while (*s++)
		++len;
	return (len);
}
