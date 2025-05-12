/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:22:11 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/05 13:38:11 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_getstrlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_calloc(size_t num, size_t size)
{
	char		*ptr;
	size_t		i;

	i = 0;
	if (num && size > LONG_MAX / num)
		return (NULL);
	ptr = malloc(num * size);
	if (!ptr)
		return (NULL);
	while (i < size)
	{
		ptr[i] = 0;
		i++;
	}
	return (ptr);
}

char	*customstrlcat(char *joined, char *s1, const char *s2)
{
	size_t	i;
	size_t	len1;
	size_t	len2;

	i = 0;
	len1 = ft_getstrlen(s1);
	len2 = ft_getstrlen(s2);
	while (i < len1)
	{
		joined[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < len2)
	{
		joined[len1 + i] = s2[i];
		i++;
	}
	return (joined);
}

char	*ft_stringjoin(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_getstrlen(s1);
	len2 = ft_getstrlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	joined = customstrlcat(joined, s1, s2);
	joined[len1 + len2] = '\0';
	return (joined);
}
