/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2023/12/19 15:14:31 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct 
{
	char	token;
	int		index;
	int		n;
} 	ast_node;

char	**parse_cmd(char *argv)
{
	while (*argv)
	{
		if (*argv == "\'")
		
		else if (*argv == "\"")
		argv++;
	}
}

// y a il deux simplequote
// y a ti deux doublequote