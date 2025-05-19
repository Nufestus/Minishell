/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 04:00:12 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/19 14:12:13 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int countquotes(char *s, bool *doublequotes, bool *singlequotes)
{
    int twoquotes;
    int onequotes;
    int res;
    int i;
    
    i = -1;
    res = 0;
    twoquotes = 0;
    onequotes = 0;
    while(s[++i])
        if (s[i] == '"')
            twoquotes++;
    i = -1;
    while(s[++i])
        if (s[i] == '\'')
            onequotes++;
    if (twoquotes % 2 != 0)
        *doublequotes = true;
    if (onequotes % 2 != 0)
        *singlequotes = true;
    return (0);
}

static size_t    count_str(char *s, char *delims)
{
    unsigned int    i;
    bool doublequotes;
    bool singlequotes;
    size_t            word;

    i = 0;
    word = 0;
    doublequotes = false;
    singlequotes = false;
    countquotes(s, &doublequotes, &singlequotes);
    if (doublequotes || singlequotes)
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

char    **ft_split(int *check, char *s, char *delims)
{
    size_t        k;
    size_t        index;
    char        **p;

    if (!s || !delims)
        return (NULL);
    k = count_str((char *)s, delims);
    if (k == 0)
    {
        if (check)
            *check = 1;
        return (NULL);
    }
    index = 0;
    p = (char **) malloc((sizeof(char *)) * (k + 1));
    if (!p)
        return (NULL);
    initializetonone(p, k);
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
