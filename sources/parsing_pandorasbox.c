/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pandorasbox.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2023/12/30 19:03:17 by svidot           ###   ########.fr       */
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
#include <stdio.h>

// t_ast_nde	*invert_node(t_ast_nde **node, char **argv)
// {	
// 	int			flag;
// 	t_ast_nde	*invrt_nde;

// 	flag = 0;
// 	invrt_nde = NULL;
// 	printf("here deb\n");
// 	//printf("here: %c, %c, %c\n",*(*argv + 1), *((*node)->start + 1), *((*node)->end - 1));
// 	if (*node && *argv >= (*node)->start && *argv <= (*node)->end && ++flag)  // si argv est compris ds un noeud au debut note le debut du invert noeud ds argv puis avance node
// 	{
// 		printf("JO: %c\n",**argv);
// 		*argv = (*node)->end + 1;
// 		return (NULL);
// 		//*node = (*node)->sibling;
// 	}
// 	if (**argv)
// 	{printf("JU: %c\n",**argv);
// 		invrt_nde = create_node(INVRT);
// 		invrt_nde->start = *argv;
// 	}
// 	if (**argv && *node)
// 	{
// 		printf("JI: %c\n", **argv);
// 		invrt_nde->end = (*node)->start - 1;

// 		*argv = (*node)->end + 1;		
// 	}
// 	else if (**argv && !*node)
// 	{	
// 		printf("ZA");
// 		printf("ZA: %c\n",**argv);
// 		while (**argv)	
// 		{
// 			printf("ZU: %c\n",**argv);
// 			invrt_nde->end = (*argv)++;	
// 		}printf("ZZ\n");
// 	}
// 	// }
// 	// if (*node && *argv >= (*node)->start && *argv <= (*node)->end && ++flag)
// 	// {
// 	// 	//printf("JU: %c\n",**argv);
// 	// 	//*argv = (*node)->end + 1;
// 	// }
// 	return (invrt_nde);
// }

t_ast_nde	*invert_node(t_ast_nde *node, char **argv)
{	
	t_ast_nde	*invrt_nde;

	if (node && *argv >= node->start && *argv <= node->end)  
	{		
		*argv = node->end + 1;
		return (NULL);	
	}
	invrt_nde = create_node(INVRT);
	invrt_nde->start = *argv;
	if (node)
	{	
		invrt_nde->end = node->start - 1;
		*argv = node->end + 1;		
	}
	while (**argv && !node)
		invrt_nde->end = (*argv)++;
	return (invrt_nde);
}
// t_ast_nde	*invert_node(t_ast_nde **node, char **argv)
// {	
// 	int			flag;
// 	t_ast_nde	*invrt_nde;

// 	flag = 0;
// 	invrt_nde = NULL;
// 	//printf("here: %c, %c, %c\n",*(*argv + 1), *((*node)->start + 1), *((*node)->end - 1));
// 	while (*node && *argv >= (*node)->start && *argv <= (*node)->end && ++flag)
// 	{
// 		printf("JO: %c\n",**argv);
// 		(*argv)++;
// 	}
// 	if (flag)
// 		*node = (*node)->sibling;
// 	if (**argv)
// 	{
// 		invrt_nde = create_node(INVRT);
// 		invrt_nde->start = *argv;
// 	}
// 	while ((**argv && *node && *argv < (*node)->start) || (**argv && !*node))	
// 		invrt_nde->end = (*argv)++;
// 	while (*node && *argv >= (*node)->start && *argv <= (*node)->end && ++flag)
// 	{
// 		printf("JU: %c\n",**argv);
// 		(*argv)++;
// 	}
// 	return (invrt_nde);
// }
	// if (node && *argv < node->start)
	// {	printf("never here 1, %c, n start %c\n", **argv, *(node->start + 1));
	// 	invrt_nde = create_node(INVRT);
	// 	invrt_nde->start = *argv;
	// 	while (**argv && *argv < node->start)
	// 	{
	// 		invrt_nde->end = *argv;
	// 		(*argv)++;
	// 	}
	// 	while (**argv && *argv <= node->end)
	// 		(*argv)++;
	// 	return (invrt_nde);
	// }
	// else if (node)
	// {	printf("never here 2, %c,  n start %c\n", **argv, *(node->start + 1));
	// 	while (**argv && *argv <= node->end)
	// 		(*argv)++;
	// 	invrt_nde = create_node(INVRT);
	// 	invrt_nde->start = *argv;
	// 	while (**argv && node->sibling && *argv < node->sibling->start)
	// 	{
	// 		invrt_nde->end = *argv;
	// 		(*argv)++;
	// 	}		
	// 	return (invrt_nde);
	// }
	// printf("never here !\n");
	// return (NULL);
	#include <stdio.h>
t_ast_nde	*filter_wrapper(char *argv, t_ast_nde *node, t_ast_nde *(*filter)(t_ast_nde *, char **))
{
	t_ast_nde	*res_nde;
	t_ast_nde	*res_sibling;
	t_ast_nde	*res_sibling_sav;

	res_sibling = NULL;
	while (node || *argv)
	{
		res_nde = filter(node, &argv);
		if (res_nde)
			add_sibling(res_nde, &res_sibling, &res_sibling_sav);
		if (node)	
			node = node->sibling;
	}
	return (res_sibling_sav);
}

void	sibling_reader(t_ast_nde *node)
{
	char	*start_sav;
	
	while (node)
	{
		start_sav = node->start;
		while (node->start <= node->end)
		{
			printf("%c", *node->start);
			node->start++;
		}
		printf("\n");
		node->start = start_sav;
		node = node->sibling;
	}
}

t_ast_nde	*set_space_nde(t_ast_nde *node)
{	
	static t_ast_nde	*spce_nde;
	static int			flag;	
	static char			*lcl_start;
	static char			*lcl_end;
	
	if (!flag)
		spce_nde = create_node(SPACE);	
	while (*node->start && node->start != node->end + 1 && ft_isspace(*node->start) && !spce_nde->start)
		node->start++;
	while (*node->start && node->start != node->end + 1 && !ft_isspace(*node->start))
	{printf("lq\n");
		if (!spce_nde->start)
			spce_nde->start = node->start;
		spce_nde->end = node->start;
		node->start++;
	}
	if (spce_nde->start && (ft_isspace(*node->start) || !*node->start))
	{ printf("la\n");
		
		// lcl_end = end;
		flag = 0;
		return (spce_nde);
	}
	// else if (spce_nde->start && !ft_isspace(*start))
	// {  printf("lb");
	// 	// lcl_start = start;
	// 	// lcl_end = end;
	// 	flag = 0;
	// 	return (spce_nde);
	// }
	printf("lc\n");
	flag = 1;
	
	return (NULL);	
}

// t_ast_nde	*set_space_nde(char *start, char *end, int flag)
// {	
// 	static t_ast_nde	*spce_nde;

// 	if (flag)
// 		spce_nde = create_node(SPACE);	
// 	while (*start && start != end + 1 && ft_isspace(*start))
// 		start++;
// 	while (*start && start != end + 1 && !ft_isspace(*start))
// 	{
// 		if (!spce_nde->start)
// 			spce_nde->start = start;
// 		spce_nde->end = start;
// 		start++;
// 	}
// 	if (spce_nde->start && ft_isspace(*start))
// 		return (spce_nde);	
// 	return (NULL);	
// }

t_ast_nde	*filter_wrapper_sp(t_ast_nde *node, t_ast_nde *(*filter)(t_ast_nde *node))
{
	t_ast_nde	*res_nde;
	t_ast_nde	*res_sibling;
	t_ast_nde	*res_sibling_sav;

	res_sibling = NULL;
	while (node)
	{	//	printf("WWWWWWWWWWW\n");
		res_nde = filter(node);
		if (res_nde)
			add_sibling(res_nde, &res_sibling, &res_sibling_sav);
		if (!res_nde)
			node = node->sibling;
	}
	return (res_sibling_sav);
}
int	main(void)
{
	char *argv = "juice' szl ut 'cc est moi k la j\"vie\"";
	t_ast_nde	*res;
	printf("%s\n", argv);
	res = set_quote_nde(argv);
	sibling_reader(res);
	printf("\nstop\n\n");	
	res = filter_wrapper(argv, res, invert_node);
	sibling_reader(res);
	printf("\nstop\n\n");	
	res = filter_wrapper_sp(res, set_space_nde);
	sibling_reader(res);
	return (0);
}

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