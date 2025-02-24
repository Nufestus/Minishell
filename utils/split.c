/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:44:09 by aammisse          #+#    #+#             */
/*   Updated: 2025/02/24 23:34:47 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static size_t    count_str(char *s, char *delims)
{
    unsigned int    i;
    size_t            word;

    i = 0;
    word = 0;
    while (s[i] != '\0')
    {
        if (!is_delim(s[i], delims) && (is_delim(s[i + 1], delims)
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
    while (s[i] && !is_delim(s[i], delims))
        i++;
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