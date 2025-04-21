#include "minishell.h"

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

char *expand(char *str, int start)
{
    int len;
    char *var;
	char *copy;

	int i = start + 1; // start after '$'
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	len = i - (start + 1);
	return ft_substr(str, start + 1, len);
}

int main()
{
	char *var;

	char *str = ft_strdup("cat ls $$$lol");
	printf("initial string : \" %s \"\n", str);
	var = expand(str, 7);
	printf("after changes : \" %s \"", var);
}