/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_build.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 15:27:14 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 11:31:48 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_BUILD_H
# define PARSING_BUILD_H

# include "parsing_utils.h"

char	**create_ast(char *argv, int fd_file[]);

#endif