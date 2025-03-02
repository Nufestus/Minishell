/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:28:23 by aammisse          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/25 16:18:29 by rammisse         ###   ########.fr       */
=======
/*   Updated: 2025/02/25 14:40:54 by aammisse         ###   ########.fr       */
>>>>>>> refs/remotes/origin/main
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_minishell mini;

	(void)ac;
	(void)av;
	mini.env = env;
	readinput(&mini);
	// parse(&mini);
}