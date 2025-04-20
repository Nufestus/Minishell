/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:16:00 by rammisse          #+#    #+#             */
/*   Updated: 2025/04/20 11:50:42 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (src[len] != '\0')
		len++;
	if (dstsize == 0)
		return (len);
	while (src[i] != '\0' && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize > 0)
		dst[i] = '\0';
	return (len);
}

static int    is_delim(char c, char *delims)
{
    while (*delims)
    {
        if (c == *delims)
            return (1);
        delims++;
    }
    return (0);
}

int countquotes(char *s)
{
    int i;
    int res;
    
    i = -1;
    res = 0;
    while(s[++i])
        if (s[i] == '"' || s[i] == '\'')
            res++;
    if (res == 0 || res % 2 == 0)
        return (1);
    return (0);
}

static size_t    count_str(char *s, char *delims)
{
    unsigned int    i;
    size_t            word;

    i = 0;
    word = 0;
    if (!countquotes(s))
        return (0);
    while (s[i] != '\0')
    {
        if (s[i] == '"' || s[i] == '\'')
        {
            word++;
            if (s[i] == '"')
            {
                i++;
                while(s[i] && s[i] != '"')
                    i++;
            }
            else if (s[i] == '\'')
            {
                i++;
                while(s[i] && s[i] != '\'')
                    i++;
            }
        }
        else if (!is_delim(s[i], delims) && (is_delim(s[i + 1], delims)
                || s[i + 1] == '\0'))
            word++;
        i++;
    }
    return (word);
}

static size_t    str_length(char const *s, char *delims)
{
    size_t    i;

    i = 0;
    if (s[i] && (s[i] == '"' || s[i] == '\''))
    {
        if (s[i] == '"')
        {
            i++;
            while(s[i] && s[i] != '"')
                i++;
        }
        else if (s[i] == '\'')
        {
            i++;
            while(s[i] && s[i] != '\'')
                i++;
        }
        i++;
    }
    else
    {
        while (s[i] && !is_delim(s[i], delims))
            i++;
    }
    return (i);
}

static char    **free_mem(char **s, int i)
{
    int    index;

    index = 0;
    while (index < i)
    {
        free(s[index]);
        index++;
    }
    free(s);
    return (NULL);
}

char    **ft_split(char const *s, char *delims)
{
    size_t        k;
    size_t        index;
    char        **p;

    if (!s || !delims)
        return (NULL);
    k = count_str((char *)s, delims);
    if (k == 0)
        return (NULL);
    index = 0;
    p = (char **) malloc((sizeof(char *)) * (k + 1));
    if (!p)
        return (NULL);
    while (index < k)
    {
        while (is_delim(*s, delims))
            s++;
        p[index] = (char *)malloc((sizeof(char) * (str_length(s, delims) + 1)));
        if (!p[index])
            return (free_mem(p, index));
        ft_strlcpy(p[index], s, str_length(s, delims) + 1);
        s = s + str_length(s, delims);
        index++;
    }
    p[index] = NULL;
    return (p);
}
