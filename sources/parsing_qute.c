/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_qute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:08:19 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 10:43:44 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_utils.h"

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

static void	set_dqute_nde(t_ast_nde *dqute, t_ast_nde *sibling_sav, char **argv, int fd_file[])
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
		usleep(500);
		ft_putstr_fd("double quote error\n", 2);
		close(fd_file[1]);
		free_sib(sibling_sav);
		free_sib(dqute);
		exit(1);
	}
}

static void	set_squte_nde(t_ast_nde *squte, t_ast_nde *sibling_sav, char **argv, int fd_file[])
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
		ft_putstr_fd("simple quote error\n", 2);
		close(fd_file[1]);
		free_sib(sibling_sav);
		free_sib(squte);
		exit(1);
	}
}

t_ast_nde	*set_quote_nde(char *argv, int fd_file[])
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
			set_squte_nde(qute_nde, qute_sibling_sav, &argv, fd_file);
			add_sibling(qute_nde, &qute_sibling, &qute_sibling_sav);			
		}
		else if (*argv == '\"')
		{
			qute_nde = create_node(DQUTE);
			set_dqute_nde(qute_nde, qute_sibling_sav, &argv, fd_file);
			add_sibling(qute_nde, &qute_sibling, &qute_sibling_sav);
		}		
		argv++;
	}
	return (qute_sibling_sav);
}
