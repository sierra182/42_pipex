/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pandorasbox.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2024/01/03 08:41:48 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "libft.h"

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
	
	qute_sibling_sav = NULL;
	//qute_sibling = NULL;
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

// t_ast_nde	*invert_node(t_ast_nde *node, char **argv) last
// {	
// 	t_ast_nde	*invrt_nde;

// 	if (node && *argv >= node->start && *argv < node->end)//<= node->end)  
// 	{		
// 		*argv = node->end;
// 		return (NULL);	
// 	}
// 	invrt_nde = create_node(INVRT);
// 	invrt_nde->start = *argv;
// 	if (node)
// 	{	
// 		invrt_nde->end = node->start;// - 1;
// 		*argv = node->end;// + 1;		
// 	}
// 	while (**argv && !node)
// 		invrt_nde->end = (*argv)++;
// 	return (invrt_nde);
// }
t_ast_nde	*invert_node(t_ast_nde *node, char **argv)
{	
	t_ast_nde	*invrt_nde;

	invrt_nde = create_node(INVRT);
	invrt_nde->start = *argv;
	if (node)
	{	
		invrt_nde->end = node->start;// - 1;
		*argv = node->end;// + 1;		
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
// t_ast_nde	*filter_wrapper(char *argv, t_ast_nde *node, t_ast_nde *(*filter)(t_ast_nde *, char **)) last
// {
// 	t_ast_nde	*res_nde;
// 	t_ast_nde	*res_sibling;
// 	t_ast_nde	*res_sibling_sav;

// 	res_sibling = NULL;
// 	while (node || *argv)
// 	{
// 		res_nde = filter(node, &argv);
// 		if (res_nde)
// 			add_sibling(res_nde, &res_sibling, &res_sibling_sav);
// 		if (node)	
// 			node = node->sibling;
// 	}
// 	return (res_sibling_sav);
// }

t_ast_nde	*filter_wrapper(char *argv, t_ast_nde *node, t_ast_nde *(*filter)(t_ast_nde *, char **))
{
	t_ast_nde	*res_nde;
	t_ast_nde	*res_sibling;
	t_ast_nde	*res_sibling_sav;

	//res_sibling = NULL;
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
// void	sibling_reader(t_ast_nde *node)
// {
// 	char	*start_sav;

// 	while (node)
// 	{
// 		start_sav = node->start;
// 		while (node->start <= node->end)
// 		{		
// 			printf("%c", *node->start);
// 			node->start++;
// 		}
// 		printf("\n");
// 		node->start = start_sav;
// 		node = node->sibling;
// 	}
// }

void	sibling_reader(t_ast_nde *node)
{
	char	*start_sav;

	while (node)
	{
		start_sav = node->start;
		while (node->start <= node->end)
		{		
			ft_putchar_fd(*node->start, 2);
			node->start++;
		}
		ft_putstr_fd("sib_reader:\n", 2);
		node->start = start_sav;
		node = node->sibling;
	}
}

t_ast_nde	*set_space_nde(t_ast_nde *node)
{	
	static t_ast_nde	*spce_nde;  // arg pour flag et inter general
	static int			flag;	
	
	if (!flag)
		spce_nde = create_node(SPACE);	
	while (node->start <= node->end && ft_isspace(*node->start))
		node->start++;
	while (node->start <= node->end && !ft_isspace(*node->start))
	{
		if (!spce_nde->start)
			spce_nde->start = node->start;
		spce_nde->end = node->start;
		node->start++;
	}
	if (spce_nde->start && (node->start <= node->end || !*node->start))
	{
		flag = 0;		
		return (spce_nde); // je garde le noeud entree et renvois le  noeud sortie
	}
	flag = 1;
	if (!spce_nde->start)
	{	
		flag = 0;
		free(spce_nde);		
	}
	return (NULL);	// je change de noeud entree et garde le noeud sortie
}

// t_ast_nde	*set_space_nde(t_ast_nde *node)
// {	
// 	static t_ast_nde	*spce_nde;
// 	static int			flag;	
	
// 	if (!flag)
// 		spce_nde = create_node(SPACE);	
// 	while (node->start <= node->end && ft_isspace(*node->start))
// 		node->start++;
// 	while (node->start <= node->end && !ft_isspace(*node->start))
// 	{
// 		if (!spce_nde->start)
// 			spce_nde->start = node->start;
// 		spce_nde->end = node->start;
// 		node->start++;
// 	}
// 	if (spce_nde->start && ((node->start <= node->end && ft_isspace(*node->start)) || !*node->start))
// 	{
// 		flag = 0;
// 		return (spce_nde); // je garde le noeud entree et renvois le  noeud sortie
// 	}
// 	flag = 1;	
// 	return (NULL);	// je change de noeud entree et garde le noeud sortie
// }

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

	//res_sibling = NULL;
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
char	clean_quotes(char *start, t_ast_nde	*qute_nde)
{	
	static t_ast_nde	*lcl_qute_nde;
	t_ast_nde			*tmp_nde;
	
	if (qute_nde && qute_nde->start)
	{
		lcl_qute_nde = qute_nde;
		return (0);	
	}
	tmp_nde = lcl_qute_nde;
	while (tmp_nde)
	{
		if (start != tmp_nde->start && start != tmp_nde->end)
			tmp_nde = tmp_nde->sibling;
		else
			return (0);
	}
	return (*start);	
}

char	*build_node(char *start, char *start_sav, char *end)
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

char	**create_array(t_ast_nde *node)
{	
	int		i;
	
	i = 0;
	while (node)
	{
		i++;
		node = node->sibling;
	}
	return (ft_calloc(sizeof(char *), i + 1));	
}

char	**build_array(t_ast_nde *node)
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

void	free_sibs(t_ast_nde *sib)
{
	t_ast_nde	*tmp;
	
	while (sib)
	{
		tmp = sib->sibling;
		free(sib);
		sib = tmp;
	}
}

char	**create_ast(char *argv)
{
	//char 		*argv = "'g'gt  'c'eszl' ut\"\" ' ces'tm'oikajviet'y'k ";
	char		**f_res;
	int	j;
	j = 0;
	//(void) j;
	t_ast_nde	*qute_sib;
	t_ast_nde	*invrt_sib;
	t_ast_nde	*spce_sib;
	//res = NULL;
	//printf("%s\n", argv);
	qute_sib = set_quote_nde(argv);
	//sibling_reader(res);
	//sleep(1);
	clean_quotes("null", qute_sib);
//	sleep(2);
//	printf("\nstop\n\n");	
	invrt_sib = filter_wrapper(argv, qute_sib, invert_node);
	//sibling_reader(res);
//	usleep(20);
//	sibling_reader(res);
//	printf("\nstop\n\n");	
	perror("test");
	spce_sib = filter_wrapper_sp(invrt_sib, set_space_nde);
//	usleep(20);
	perror("test2");
	ft_putstr_fd("quoi\n", 2);
//	sibling_reader(res);
//	printf("\nstop\n\n");	
	f_res = build_array(spce_sib);
	while (f_res && f_res[j])	
	{		
 		ft_putstr_fd(f_res[j++], 2);
		ft_putstr_fd("\n", 2);			
	}	
	free_sibs(qute_sib);
	free_sibs(invrt_sib);
	free_sibs(spce_sib);
	// while (*f_res)	
	// 	ft_putstr_fd(*f_res++, 2);
	return (f_res);
}

char	**parse_cmd(char *argv[], char *envp[], int fd_file[])
{
	char	**split_arg;
	char	**split_colon;
	char	**split_colon_sav;
	char	*env_to_find;
	char	*env_find;
	char	*cmd;
	
	env_to_find = "PATH=";
	env_find = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp++, env_to_find, ft_strlen(env_to_find)))
		{
			env_find = *--envp;			
			break ;
		}
	}
	if (!env_find)
		return (perror("env Path not found"), exit(1), NULL);	// gerer pas de envpath !!!!!!!!!!! ++ fds
	split_arg = create_ast(*argv);//ft_split(*argv, ' ');	
	env_find += ft_strlen(env_to_find);
	split_colon = ft_split(env_find, ':');
	split_colon_sav = split_colon;
	while (*split_colon)
	{
		char	*s1;

		s1 = ft_strjoin(*split_colon++, "/");
		cmd = ft_strjoin(s1, *split_arg);
		free(s1);	
		if (!access(cmd, X_OK))
		{
			free(*split_arg);
			*split_arg = cmd;
			break;
		}
		free(cmd);
		cmd = NULL;
	}
	if (!cmd)
	{
		close(fd_file[1]);
		int i;
		i = 0;
		while (split_colon_sav[i])
			free(split_colon_sav[i++]);
		free(split_colon_sav);
		perror(*split_arg); // command not found
		i = 0;
		while (split_arg[i])
			free(split_arg[i++]);
		free(split_arg);	
		return (exit(1), NULL);
	}	
	return (split_arg);
}
// int	main(void)
// {
// 	char 		*argv = "'g'gt  'c'eszl' ut\"\" ' ces'tm'oikajviet'y'k ";
// 	t_ast_nde	*res;
// 	printf("%s\n", argv);
// 	res = set_quote_nde(argv);
// 	sibling_reader(res);
// 	clean_quotes(NULL, res);
// 	printf("\nstop\n\n");	
// 	res = filter_wrapper(argv, res, invert_node);
// 	sibling_reader(res);
// 	printf("\nstop\n\n");	
// 	res = filter_wrapper_sp(res, set_space_nde);
// 	sibling_reader(res);
// 	printf("\nstop\n\n");	
// 	char		**f_res;
// 	f_res = build_array(res);
// 	while (*f_res)	
// 		printf("%s\n", *f_res++);	
// 	return (0);
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