/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 15:27:14 by svidot            #+#    #+#             */
/*   Updated: 2023/12/21 17:09:12 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETUP_H
# define SETUP_H

void	set_filepaths(int *argc, char **argv[], char *filepaths[]);
void	get_fdio(int flag, char *filepaths[], int fd_file[]);
void	here_doc_handle(char **argv[], int pipefd_in[]);

#endif
