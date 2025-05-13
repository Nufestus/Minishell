/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:16:00 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/13 18:18:07 by rammisse         ###   ########.fr       */
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

// int countquotes(char *s, bool *doublequotes, bool *singlequotes)
// {
//     int twoquotes;
//     int onequotes;
//     int res;
//     int i;
	
//     i = -1;
//     res = 0;
//     twoquotes = 0;
//     onequotes = 0;
//     while(s[++i])
//         if (s[i] == '"')
//             twoquotes++;
//     i = -1;
//     while(s[++i])
//         if (s[i] == '\'')
//             onequotes++;
//     if (twoquotes % 2 != 0)
//         *doublequotes = true;
//     if (onequotes % 2 != 0)
//         *singlequotes = true;
//     return (0);
// }

long long    count_str(char *s, char *delims)
{
	int i = 0;
	bool in_token = false;
	int word_count = 0;
	char quote;
	
	while (s[i])
	{
	    // Skip delimiters
	    if (is_delim(s[i], delims))
	    {
	        if (in_token)
	        {
	            word_count++;
	            in_token = false;
	        }
	        i++;
	        continue;
	    }
	
	    // We’re inside a token now
	    in_token = true;
	
	    // Handle quoted tokens
	    if (s[i] == '"' || s[i] == '\'')
	    {
	        quote = s[i];
	        i++;
	        while (s[i] && s[i] != quote)
	            i++;
	        if (s[i] == quote)
	            i++; // skip closing quote
	    }
	    else
	    {
	        while (s[i] && !is_delim(s[i], delims) && s[i] != '\'' && s[i] != '"')
	            i++;
	    }
	}

// Final word if it ends without delimiter
	if (in_token)
    	word_count++;
	return (word_count);
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
	long long        k;
	size_t        index;
	char        **p;

	if (!s || !delims)
		return (NULL);
	k = count_str((char *)s, delims);
	if (k == -5)
	{
		if (check)
			*check = 1;
		return (NULL);
	}
	index = 0;
	p = (char **) malloc((sizeof(char *)) * (k + 1));
	if (!p)
		return (NULL);
	// initializetonone(p, k);
	while ((long long)index < k)
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

// int main()
// {
// 	char **s;
// 	char *str = malloc(10000);
	
// 	str = "echo $$123USER$$123\"'\"$$$$USER\"$123'\"'\"'$$$$'\"$HOME'\"'\"\"'\"$$$$678'HOME' > out1";
// 	s = ft_split(NULL, str, " \t\n\r\v\f");
// 	while(*s)
// 	{
// 		printf("%s\n", *s);
// 		s++;
// 	}
// }