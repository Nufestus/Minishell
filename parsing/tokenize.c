/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rammisse <rammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:04:21 by aammisse          #+#    #+#             */
/*   Updated: 2025/05/17 19:03:49 by rammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    setupnode(int inquotes, int index, int category, int type, char *str, t_tokenize **tokens)
{
    t_tokenize *newnode;

    newnode = ft_lstnew(ft_lstlast(*tokens), type);
    newnode->inquotes = inquotes;
    newnode->index = index;
    newnode->str = ft_strdup(str);
    newnode->category = category;
    ft_lstadd_back(tokens, newnode);
}

void syntaxhere(int *check, char *flag, int print)
{
    if (print)
    {
        if (flag)
            printf("shell: syntax error near unexpected token '%s'\n", flag);
    }
    if (check)
        *check = 1;
}

void syntax(int *check, char *flag, int print)
{
    if (print)
    {
        if (flag)
        {
            flag = ft_strjoin("shell: syntax error near unexpected token ", flag);
            printf("%s\n", flag);
            free(flag);
        }
    }
    if (check)
        *check = 1;
}

int parsepipe(t_tokenize *list)
{
    if (list->prev && list->prev->type != WORD)
        return (0);
    else if (list->next && list->next->type == PIPE)
        return (0);
    return (1);
}

int parseinput(t_tokenize *list)
{
    if (list->next && list->next->type != WORD)
        return (0);
    else if (!list->next)
        return (0);
    return (1);
}

int parseoutput(t_tokenize *list)
{
    if (list->prev && list->prev->type != WORD && list->prev->type != PIPE)
        return (0);
    else if (!list->next)
        return (0);
    return (1);
}

void retokenize(t_minishell *mini)
{
    t_tokenize *list;

    list = mini->tokens;
    while(list)
    {
        if (list->type == ARG && list->prev && 
            (list->prev->type == FILE || list->prev->type == PIPE) 
                && ((list->next && (list->next->type == ARG || list->next->category)) || !list->next))
            list->type = CMD;
        list = list->next;
    }
}

void tokenizewords(t_minishell *mini)
{
    t_tokenize *list;

    list = mini->tokens;
    while (list)
    {
        if (list->type != WORD)
        {
            list = list->next;
            continue;
        }
        if (list->prev && list->prev->category && list->type == WORD && list->prev->type != HEDOC)
            list->type = FILE;
        else if ((!list->prev || list->prev->type == PIPE || list->prev->type == DEL)
            && list->type == WORD)
            list->type = CMD;
        else if (list->prev && list->prev->type == CMD && list->type == WORD)
            list->type = ARG;
        else if (list->prev && list->prev->type == HEDOC && list->type == WORD)
            list->type = DEL;
        else if (list->prev && list->prev->type == OPTION)
            list->type = ARG;
        else if (list->prev && list->prev->type == ARG && list->next && list->next->category)
            list->type = ARG;
        else
            list->type = ARG;
        list = list->next;
    }
}

int isanoption(char *str)
{
    int i;

    i = 0;
    if (str[i] == '-')
    {
        i++;
        while(str[i] && str[i] == 'n')
            i++;
    }
    if (str[i] == '\0' && i != 0)
        return (1);
    return (0);
}

// void tokenizesymbols(char **str, t_tokenize **tokens)
// {
//     int i;

//     i = 0;
//     while (str[i])
//     {
//         if (!ft_strcmp(str[i], "|"))
//             setupnode(i, 0, PIPE, str[i], tokens);
//         else if (!ft_strcmp(str[i], ">>"))
//             setupnode(i, 1, APPEND, str[i], tokens);
//         else if (!ft_strcmp(str[i], "<<"))
//             setupnode(i, 1, HEDOC, str[i], tokens);
//         else if (!ft_strcmp(str[i], ">"))
//             setupnode(i, 1, REDOUT, str[i], tokens);
//         else if (!ft_strcmp(str[i], "<"))
//             setupnode(i, 1, REDIN, str[i], tokens);
//         else if (isanoption(str[i]))
//             setupnode(i, 0, OPTION, str[i], tokens);
//         else
//             setupnode(i, 0, WORD, str[i], tokens);
//         i++;
//     }
// }

char *handletypes(int i)
{
    if (i == WORD)
        return ("WORD");
    else if (i == OPTION)
        return ("OPTION");
    else if (i == CMD)
        return ("CMD");
    else if (i == REDOUT)
        return ("REDOUT");
    else if (i == REDIN)
        return ("REDIN");
    else if (i == APPEND)
        return ("APPEND");
    else if (i == HEDOC)
        return ("HEDOC");
    else if (i == PIPE)
        return ("PIPE");
    else if (i == FILE)
        return ("FILE");
    else if (i == ARG)
        return ("ARG");
    else if (i == DEL)
        return ("DEL");
    return ("NULL");
}

bool	is_token(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

char	*fillspace(const char *input)
{
	int		i;
	int		j;
	char	*out;

    j = 0;
    i = 0;
    out = malloc(strlen(input) * 3 + 1);
	if (!out)
		return NULL;
	while (input[i])
    {
		if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
        {
			if (j > 0 && (out[j - 1] != ' ' && out[j - 1] != '"' && out[j - 1] != '\''))
				out[j++] = ' ';
			out[j++] = input[i++];
			out[j++] = input[i++];
			if (input[i] && (input[i] != ' ' && input[i] != '"' && input[i] != '\''))
				out[j++] = ' ';
		}
		else if (is_token(input[i]))
        {
			if (j > 0 && (out[j - 1] != ' ' && out[j - 1] != '"' && out[j - 1] != '\''))
				out[j++] = ' ';
			out[j++] = input[i++];
			if (input[i] && (input[i] != ' ' && input[i] != '"' && input[i] != '\''))
				out[j++] = ' ';
		}
		else
			out[j++] = input[i++];
	}
	out[j] = '\0';
	return out;
}

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

static int	count_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static char	*rtrn(int n, char *num, size_t len)
{
	unsigned int	abs;
	char			*index;

	index = "0123456789";
	num[len--] = '\0';
	if (n == 0)
	{
		num[0] = '0';
		return (num);
	}
	if (n < 0)
	{
		num[0] = '-';
		abs = -n;
	}
	else
		abs = n;
	while (abs > 0)
	{
		num[len--] = index[abs % 10];
		abs /= 10;
	}
	return (num);
}

char	*ft_itoa(int n)
{
	char	*num;
	size_t	len;

	len = count_len(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	num = (char *) malloc (len + 1);
	if (num == NULL)
		return (NULL);
	num = rtrn(n, num, len);
	return (num);
}

static char	*alloc(char *str)
{
	int		i;
	char	*string;

	i = 0;
	string = (char *)malloc(ft_strlen(str) + 1);
	if (!string)
		return (NULL);
	while (str[i] != '\0')
	{
		string[i] = str[i];
		i++;
	}
	string[i] = '\0';
	return (string);
}

static int	in_set(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char *s1, char *set)
{
	size_t	startind;
	size_t	lastind;
	char	*trim;

	if (s1 != NULL && set == NULL)
		return (alloc((char *)s1));
	else if ((s1 == NULL && set == NULL) || (s1 == NULL && set != NULL))
		return (NULL);
	startind = 0;
	lastind = ft_strlen((char *)s1) - 1;
	while (in_set(s1[startind], (char *)set) && s1[startind] != '\0')
		startind++;
	if (s1[startind] == '\0')
		return (ft_strdup(""));
	while (in_set(s1[lastind], (char *)set) && lastind > startind)
		lastind--;
	trim = (char *)malloc(lastind - startind + 2);
	if (trim == NULL)
		return (NULL);
	ft_strlcpy(trim, s1 + startind, lastind - startind + 2);
	return (trim);
}

int countdouble(char *str, char *delims)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (!is_delim(str[i], delims) && (is_delim(str[i + 1], delims)
                || str[i + 1] == '\0'))
            count++;
        i++;
    }
    return (count);
}

int countword(char **str)
{
    int i;
    int k;
    int copy;
    char q;
    char *s;

    i = 0;
    k = 0;
    s = *str;
    while (s[i])
    {
        while (s[i] && ft_strchr(" \t\n\r\v\f", s[i]))
            i++;
        if (!s[i])
            break ;
        k = 0;
        while (s[i] && !ft_strchr(" \t\n\r\v\f", s[i]))
        {
            if (s[i] == '"' || s[i] == '\'')
            {
                q = s[i];
                i++;
                k++;
                while (s[i] && s[i] != q)
                {
                    k++;
                    i++;
                }
                if (s[i] == q)
                {
                    i++;
                    k++;
                }
            }
            else
            {
                k++;
                i++;
            }
        }
        copy = i;
        while(copy > 0)
        {
            (*str)++;
            copy--;
        }
        return (k);
    }
    return (k);
}

void handle(int inquotes, char *str, int i, t_tokenize **tokens)
{
    if (!ft_strcmp(str, "|"))
        setupnode(inquotes, i, 0, PIPE, str, tokens);
    else if (!ft_strcmp(str, ">>"))
        setupnode(inquotes, i, 1, APPEND, str, tokens);
    else if (!ft_strcmp(str, "<<"))
        setupnode(inquotes, i, 1, HEDOC, str, tokens);
    else if (!ft_strcmp(str, ">"))
        setupnode(inquotes, i, 1, REDOUT, str, tokens);
    else if (!ft_strcmp(str, "<"))
        setupnode(inquotes, i, 1, REDIN, str, tokens);
    else if (isanoption(str))
        setupnode(inquotes, i, 0, OPTION, str, tokens);
    else
        setupnode(inquotes, i, 0, WORD, str, tokens);
}

void    ft_reparse(int *check, char *str, t_minishell *mini)
{
    int flag;
    int     i;
    int z;
    int j;
    int k;
    char q;
    char *token;
    char *copy;

    i = 0;
    j = 0;
    copy = str;
    while (str[i])
    {
        flag = 0;
        z = countword(&copy);
        while (str[i] && ft_strchr(" \t\n\r\v\f", str[i]))
        i++;
        if (!str[i])
        break ;
        token = malloc(z + 1);
        if (!token)
        return ;
        k = 0;
        while (str[i] && !ft_strchr(" \t\n\r\v\f", str[i]))
        {
            if (str[i] == '"' || str[i] == '\'')
            {
                q = str[i];
                token[k++] = str[i++];
                while (str[i] && str[i] != q)
                    token[k++] = str[i++];
                if (str[i] == q)
                    token[k++] = str[i++];
                else if (str[i] != q)
                {
                    *check = 1;
                    free(token);
                    free(str);
                    return ;
                }
            }
            else
                token[k++] = str[i++];
        }
        token[k] = '\0';
        token = expand(&flag, token, mini);
        if (flag)
        {
            k = 0;
            char **s = ft_split(NULL, token, " \t\n\r\v\f");
            while(s[k])
                handle(0, s[k++], j++, &mini->tokens);
        }
        else
            handle(0, token, j++, &mini->tokens);
        free(token);
    }
}


int tokenize(t_minishell *mini)
{
    int i;
    int check;
    char *addspaces;

    i = 0;
    check = 0;
    mini->tokens = NULL;
    addspaces = fillspace(mini->input);

    if (addspaces == NULL)
        return (-1);
    ft_reparse(&check, addspaces, mini);
    if (check)
    {
        free(addspaces);
        if (check == 2)
            return (-1);
        else if (check == 1)
        {
            syntax(NULL, "'quotes'", 1);
            return (-1);
        }
    }
    tokenizewords(mini);
    retokenize(mini);
    free(addspaces);
    return (0);
}
