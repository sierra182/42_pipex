/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:08:19 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 15:15:50 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parsing_qute.h"
#include "parsing_invrt.h"
#include "parsing_spce.h"

static char	*build_node(char *start, char *start_sav, char *end)
{
	char	*str;
	char	*str_sav;
	char	c;
	int		i;

	i = 0;
	while (start <= end)
		if (clean_quotes(start++, NULL))
			i++;
	str = (char *) ft_calloc(i + 1, sizeof(char));
	str_sav = str;
	while (start_sav <= end)
	{
		c = clean_quotes(start_sav++, NULL);
		if (c)
			*str++ = c;
	}
	return (str_sav);
}

static char	**create_array(t_ast_nde *node)
{
	int	i;

	i = 0;
	while (node && ++i)
		node = node->sibling;
	return (ft_calloc(sizeof(char *), i + 1));
}

static char	**build_array(t_ast_nde *node)
{
	char	**array;
	char	**array_sav;

	array = create_array(node);
	array_sav = array;
	while (node)
	{
		*array++ = build_node(node->start, node->start, node->end);
		node = node->sibling;
	}
	return (array_sav);
}

char	**create_ast(char *argv, int fd_file[])
{
	char		**ast_res;
	t_ast_nde	*qute_sib;
	t_ast_nde	*invrt_sib;
	t_ast_nde	*spce_sib;

	qute_sib = set_quote_nde(argv, fd_file);
	invrt_sib = filter_wrapper_invrt(argv, qute_sib, invert_node);
	spce_sib = filter_wrapper_spce(invrt_sib, set_space_nde);
	clean_quotes("null", qute_sib);
	ast_res = build_array(spce_sib);
	free_sib(qute_sib);
	free_sib(invrt_sib);
	free_sib(spce_sib);
	return (ast_res);
}
