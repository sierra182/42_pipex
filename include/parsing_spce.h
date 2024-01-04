/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_spce.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 15:27:14 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 11:33:14 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_SPCE_H
# define PARSING_SPCE_H

# include "parsing_utils.h"

t_ast_nde	*set_space_nde(t_ast_nde *node);
t_ast_nde	*filter_wrapper_spce(t_ast_nde *node,
				t_ast_nde *(*filter)(t_ast_nde *node));

#endif
