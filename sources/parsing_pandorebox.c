/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2023/12/19 16:12:15 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

typedef struct s_ast_node
{
	char			token;
	int				start;
	int				end;
	struct ast_node	*child;
	struct ast_node	*sibling;
}	t_ast_node;

t_ast_node	*create_node(void)
{
	t_ast_node	*new_node;
	
	new_node = (t_ast_node *) (1, sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
}

char	**parse_cmd(char *argv)
{
	t_ast_node	*node;
	
	node = create_node();
	while (*argv)
	{
		if (*argv == "\'")
		
		else if (*argv == "\"")
		argv++;
	}
}

// y a il deux simplequote
// y a ti deux doublequote