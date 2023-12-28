/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pandorasbox.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2023/12/28 17:04:34 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

typedef enum e_tok
{
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
	struct ast_node	*child;
	struct ast_node	*sibling;
}	t_ast_nde;

t_ast_node	*create_node(t_tok token)
{
	t_ast_node	*new_node;

	new_node = (t_ast_nde *) ft_calloc(1, sizeof(t_ast_nde));
	if (!new_node)
		return (NULL);
	new_node->token = token;
	return (new_node);
}
int	set_squte_nde(t_ast_nde squte, char *argv)
{
	while (*argv)
	{
		if (*argv == '\'')
		{
			if (!cmd->start)
				cmd->start = argv;
			else
			{
				cmd->end = argv;
				break ;
			}
		}
		argv++;
	}
	if (cmd->start && !cmd->end)
	{
		ft_putstr_fd("simple quote error.\n", 2); // gerer err
		return (-1);
	}
	else if (!cmd->start && !cmd->end)
		return (0);
	else if (cmd->start && cmd->end)
		return (1);
}


t_ast_nde	*set_space_nde(char *start, char *end, int flag)
{	
	static t_ast_nde	*spce_nde;

	if (flag)
		*spce_nde = create_node(SPACE);	
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

t_ast_nde	*filter_wrapper(t_ast_nde node, t_ast_nde *(filter *)(char *, char *))
{
	t_ast_nde	*res_nde;
	t_ast_nde	*res_sibling;
	t_ast_nde	*res_sibling_sav;

	while (space)
	{
		res_nde = filter(space->start, space->end);
		if (res_nde)
			if (!res_sibling)
			{
				res_sibling = res_nde;
				res_sibling_sav = res_sibling;
			}
			else
			{
				res_sibling->sibling = res_nde;
				res_sibling = res_sibling->sibling;
			}
		space = space->sibling;
	}
	return (res_sibling_sav);
}

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