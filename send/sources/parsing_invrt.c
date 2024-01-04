/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_invrt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:08:19 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 15:17:11 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "parsing_utils.h"

t_ast_nde	*invert_node(t_ast_nde *node, char **argv)
{
	t_ast_nde	*invrt_nde;

	invrt_nde = create_node(INVRT);
	invrt_nde->start = *argv;
	if (node)
	{
		invrt_nde->end = node->start;
		*argv = node->end;
	}
	while (**argv && !node)
		invrt_nde->end = (*argv)++;
	return (invrt_nde);
}

t_ast_nde	*filter_wrapper_invrt(char *argv, t_ast_nde *node,
									t_ast_nde *(*filter)(t_ast_nde *, char **))
{
	t_ast_nde	*res_nde;
	t_ast_nde	*res_sibling;
	t_ast_nde	*res_sibling_sav;

	res_sibling_sav = NULL;
	while (node || *argv)
	{
		res_nde = filter(node, &argv);
		add_sibling(res_nde, &res_sibling, &res_sibling_sav);
		if (node)
			node = node->sibling;
	}
	return (res_sibling_sav);
}
