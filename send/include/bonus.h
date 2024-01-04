/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 09:57:09 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 10:01:31 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H
# define BONUS 0
# ifdef EN_BONUS
#  include "get_next_line.h"
#  undef BONUS
#  define BONUS 1
# endif
#endif