/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:17:41 by rammisse          #+#    #+#             */
/*   Updated: 2025/05/27 01:04:33 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fillspacehelp(int *j, int *i, const char *input, char *out)
{
	if (*j > 0 && out[*j - 1] != ' ')
		out[(*j)++] = ' ';
	out[(*j)++] = input[(*i)++];
	out[(*j)++] = input[(*i)++];
	if (input[*i] && input[*i] != ' ')
		out[(*j)++] = ' ';
}

void	fillspacehelp2(int *j, int *i, char *out, const char *input)
{
	if (*j > 0 && out[*j - 1] != ' ')
		out[(*j)++] = ' ';
	out[(*j)++] = input[(*i)++];
	if (input[*i] && input[*i] != ' ')
		out[(*j)++] = ' ';
}

char	*fillspace(const char *input)
{
	int		i;
	int		j;
	int		inquote;
	char	*out;

	j = 0;
	i = 0;
	inquote = 0;
	out = malloc(strlen(input) * 3 + 1);
	if (!out)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			inquote = !inquote;
		if ((input[i] == '<' || input[i] == '>')
			&& input[i + 1] == input[i] && !inquote)
			fillspacehelp(&j, &i, input, out);
		else if (is_token(input[i]) && !inquote)
			fillspacehelp2(&j, &i, out, input);
		else
			out[j++] = input[i++];
	}
	out[j] = '\0';
	return (out);
}

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}
