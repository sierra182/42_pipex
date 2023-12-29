/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pandorasbox.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2023/12/29 15:03:35 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../ft_printf/libft/libft.h"

typedef enum e_tok
{
	INVRT,
	SQUTE,
	DQUTE,
	SPACE,
	CMD,
	ARG_CMD,
	OPT_CMD,
	ARG_OPT	
}	t_tok;

typedef struct s_ast_nde
{
	t_tok			token;
	char			*start;
	char			*end;
	struct s_ast_nde	*child;
	struct s_ast_nde	*sibling;
}	t_ast_nde;

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
	if (!*sibling)			
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

int	set_squte_nde(t_ast_nde *squte, char **argv)
{
	while (**argv)
	{
		if (**argv == '\'')
		{
			if (!squte->start)
				squte->start = *argv;
			else
			{
				squte->end = *argv;
				break ;
			}
		}
		(*argv)++;
	}
	if (squte->start && !squte->end)
	{
		ft_putstr_fd("simple quote error.\n", 2); // gerer err
		return (-1);
	}
	else if (!squte->start && !squte->end)
		return (0);
	else if (squte->start && squte->end)
		return (1);
	return (0);
}

int	set_dqute_nde(t_ast_nde *dqute, char **argv)
{
	while (**argv)
	{
		if (**argv == '\"')
		{
			if (!dqute->start)
				dqute->start = *argv;
			else
			{
				dqute->end = *argv;
				break ;
			}
		}
		(*argv)++;
	}
	if (dqute->start && !dqute->end)
	{
		ft_putstr_fd("double quote error.\n", 2); // gerer err
		return (-1);
	}
	else if (!dqute->start && !dqute->end)
		return (0);
	else if (dqute->start && dqute->end)
		return (1);
	return (0);
}
t_ast_nde	*set_quote_nde(char *argv)
{
	t_ast_nde	*qute_sibling_sav;
	t_ast_nde	*qute_sibling;
	t_ast_nde	*qute_nde;
	
	qute_sibling = NULL;
	while (*argv)
	{
		if (*argv == '\'')
		{
			qute_nde = create_node(SQUTE);
			if (set_squte_nde(qute_nde, &argv))
				add_sibling(qute_nde, &qute_sibling, &qute_sibling_sav);
		}
		else if (*argv == '\"')
		{
			qute_nde = create_node(DQUTE);
			if (set_dqute_nde(qute_nde, &argv))
				add_sibling(qute_nde, &qute_sibling, &qute_sibling_sav);
		}		
		argv++;
	}
	return (qute_sibling_sav);
}

t_ast_nde	*invert_node(t_ast_nde *node, char *argv)
{
	static char	*lcal_argv; 
	t_ast_nde	*invrt_nde;
	
	if (!lcal_argv)
		lcal_argv = argv;
	if (node && node->start < lcal_argv)
	{
		invrt_nde = create_node(INVRT);
		invrt_nde->start = lcal_argv;
		while (*lcal_argv && lcal_argv < node->start)
		{
			invrt_nde->end = lcal_argv;
			lcal_argv++;
		}
		if (!*lcal_argv)
			lcal_argv = NULL;
		return (invrt_nde);	
	}
	else if (node)
	{
		while (*lcal_argv && lcal_argv <= node->end)
			lcal_argv++;
		invrt_nde = create_node(INVRT);
		invrt_nde->start = lcal_argv;
		while (*lcal_argv && node->sibling && lcal_argv <= node->sibling->start)
		{
			invrt_nde->end = lcal_argv;
			lcal_argv++;
		}
		if (!*lcal_argv)
			lcal_argv = NULL;
		return (invrt_nde);
	}
	return (NULL);
}

t_ast_nde	*filter_wrapper(char *argv, t_ast_nde *node, t_ast_nde *(*filter)(t_ast_nde *, char *))
{
	t_ast_nde	*res_nde;
	t_ast_nde	*res_sibling;
	t_ast_nde	*res_sibling_sav;

	res_sibling = NULL;
	while (node)
	{				
		res_nde = filter(node, argv);
		if (res_nde)
			add_sibling(res_nde, &res_sibling, &res_sibling_sav);		
		node = node->sibling;
	}
	return (res_sibling_sav);
}
#include <stdio.h>

void	sibling_reader(t_ast_nde *node)
{
	while (node)
	{
		while (node->start <= node->end)
		{
			printf("%c", *node->start);
			node->start++;
		}
		printf("\n");
		node = node->sibling;
	}
}

int	main(void)
{
	char *argv = "\"sal'ut\" c est 'moi' la 'vie'";
	t_ast_nde	*res;
	res = set_quote_nde(argv);
	sibling_reader(res);
	printf("\n");
	res = filter_wrapper(argv, res, invert_node);
	sibling_reader(res);
	return (0);
}
/*
t_ast_nde	*set_space_nde(char *start, char *end, int flag)
{	
	static t_ast_nde	*spce_nde;

	if (flag)
		spce_nde = create_node(SPACE);	
	while (*start && start != end + 1 && ft_isspace(*start))
		start++;
	while (*start && start != end + 1 && !ft_isspace(*start))
	{
		if (!spce_nde->start)
			spce_nde->start = start;
		spce_nde->end = start;
		start++;
	}
	if (spce_nde->start && ft_isspace(*start))
		return (spce_nde);	
	return (NULL);	
}

*/

// t_ast_nde	*filter_wrapper(t_ast_nde node, t_tok token, t_ast_nde *(filter *)(char *, char *))
// {
// 	t_ast_nde	*res_nde;
// 	t_ast_nde	*res_sibling;
// 	t_ast_nde	*res_sibling_sav;

// 	res_sibling = NULL;
// 	while (node)
// 	{
// 		if (node->token == token)
// 		{			
// 			res_nde = filter(node->start, node->end);
// 			if (res_nde)
// 				add_sibling(res_nde, &res_sibling, &res_sibling_sav);
// 		}
// 		node = node->sibling;
// 	}
// 	return (res_sibling_sav);
// }


/*
void	set_token(t_ast_nde seek, char *argv)
{
	if (set_squte_nde(seek, argv))
	{
		seek->token = SQUTE;
		seek->child = create_node(CMD);
	}
	else
	{
		seek->token = 0;
		seek->start = NULL;	
		seek->end = NULL;	
	}
	seek->token = CMD;
	set_cmd_nde(seek, argv);
	seek->child = create_node();
}

char	**parse_cmd(char *argv)
{
	t_ast_nde	*seek;
	
	seek = create_node(0);
	while (*argv)
	{
		if (*argv == "\'")
		
		else if (*argv == "\"")
		argv++;
	}
}
void	set_cmd_nde(t_ast_nde cmd, char *argv)
{
	while (*argv)
	{
		while (*argv && !ft_isspace(*argv))
		{
			if (!cmd->start)
				cmd->start = argv;
			cmd->end = argv;
			argv++;
		}
		if (cmd->start)
			break ;
		argv++;
	}
	if (!cmd->start)
		ft_putstr_fd("no cmd found.\n", 2); // gerer err
}

// y a il deux simplequote
// y a ti deux doublequote
*/