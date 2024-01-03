/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:08:19 by svidot            #+#    #+#             */
/*   Updated: 2024/01/03 19:46:05 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ast_nde	*create_node(t_tok token)
{
	t_ast_nde	*new_node;

	new_node = (t_ast_nde *) ft_calloc(1, sizeof(t_ast_nde));
	if (!new_node)
		return (NULL);
	new_node->token = token;
	return (new_node);
}

void	add_sibling(t_ast_nde *node, t_ast_nde **sibling, t_ast_nde **sibling_sav)
{
	if (!*sibling_sav)			
	{
		*sibling = node;
		*sibling_sav = *sibling;
	}
	else
	{
		(*sibling)->sibling = node;
		*sibling = (*sibling)->sibling;
	}
}

void	free_sib(t_ast_nde *sib)
{
	t_ast_nde	*tmp;

	while (sib)
	{
		tmp = sib->sibling;
		free(sib);
		sib = tmp;
	}
}

void	free_ptr_arr(char **arr)
{
	int i;

	if (arr)
	{		
		i = 0;
		while (arr[i])
			free(arr[i++]);
		free(arr);
	}
}