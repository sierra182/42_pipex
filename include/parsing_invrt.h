/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_invrt.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 15:27:14 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 11:31:07 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_INVRT_H
# define PARSING_INVRT_H

# include "parsing_utils.h"

t_ast_nde	*invert_node(t_ast_nde *node, char **argv);
t_ast_nde	*filter_wrapper_invrt(char *argv, t_ast_nde *node,
				t_ast_nde *(*filter)(t_ast_nde *, char **));

#endif