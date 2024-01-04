/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_spce.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:08:19 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 15:15:15 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "parsing_utils.h"

static void	*free_spce_nde_if_empty(t_ast_nde *spce_nde, int *flag)
{
	if (!spce_nde->start)
	{
		*flag = 0;
		free(spce_nde);
	}
	return (NULL);
}

t_ast_nde	*set_space_nde(t_ast_nde *node)
{
	static t_ast_nde	*spce_nde;
	static int			flag;

	if (!flag)
		spce_nde = create_node(SPACE);
	while (node->start <= node->end && ft_isspace(*node->start))
		node->start++;
	while (node->start <= node->end && !ft_isspace(*node->start))
	{
		if (!spce_nde->start)
			spce_nde->start = node->start;
		spce_nde->end = node->start++;
	}
	if (spce_nde->start && (node->start <= node->end || !*node->start))
	{
		flag = 0;
		return (spce_nde);
	}
	flag = 1;
	return (free_spce_nde_if_empty(spce_nde, &flag));
}

t_ast_nde	*filter_wrapper_spce(t_ast_nde *node,
									t_ast_nde *(*filter)(t_ast_nde *node))
{
	t_ast_nde	*res_nde;
	t_ast_nde	*res_sibling;
	t_ast_nde	*res_sibling_sav;

	res_sibling_sav = NULL;
	while (node)
	{
		res_nde = filter(node);
		if (res_nde)
			add_sibling(res_nde, &res_sibling, &res_sibling_sav);
		if (!res_nde)
			node = node->sibling;
	}
	return (res_sibling_sav);
}
