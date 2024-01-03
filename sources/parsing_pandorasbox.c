/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pandorasbox.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2024/01/03 11:29:29 by svidot           ###   ########.fr       */
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

t_ast_nde	*filter_wrapper(char *argv, t_ast_nde *node, t_ast_nde *(*filter)(t_ast_nde *, char **))
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
		spce_nde->end = node->start;
		node->start++;
	}
	if (spce_nde->start && (node->start <= node->end || !*node->start))
	{
		flag = 0;		
		return (spce_nde);
	}
	flag = 1;
	if (!spce_nde->start)
	{	
		flag = 0;
		free(spce_nde);		
	}
	return (NULL);
}

t_ast_nde	*filter_wrapper_sp(t_ast_nde *node, t_ast_nde *(*filter)(t_ast_nde *node))
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
	char		**ast_res;
	t_ast_nde	*qute_sib;
	t_ast_nde	*invrt_sib;
	t_ast_nde	*spce_sib;
	
	qute_sib = set_quote_nde(argv);
	invrt_sib = filter_wrapper(argv, qute_sib, invert_node);
	spce_sib = filter_wrapper_sp(invrt_sib, set_space_nde);
	clean_quotes("null", qute_sib);
	ast_res = build_array(spce_sib);
	free_sibs(qute_sib);
	free_sibs(invrt_sib);
	free_sibs(spce_sib);
	return (ast_res);
}

char	*search_path(char *envp[])
{
	char	*env_to_find;
	char	*env_find;

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
		return (ft_putstr_fd("env PATH not found.\n", 2), close(fd_file[1]), exit(1), NULL);
	env_find += ft_strlen(env_to_find);
	return (env_find)
}

char	*try_paths(char **split_colon, char **split_arg, char *cmd)
{	
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
	return (cmd);
}

char	**parse_cmd(char *argv[], char *envp[], int fd_file[])
{
	char	**split_arg;
	char	**split_colon;
	char	*env_find;
	char	*cmd;

	env_find = search_path(envp);
	if (!env_find)
		return (ft_putstr_fd("env PATH not found.\n", 2), close(fd_file[1]), exit(1), NULL);
	split_arg = create_ast(*argv);
	split_colon = ft_split(env_find, ':');
	cmd = try_paths(split_colon, split_arg, cmd);
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
