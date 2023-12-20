/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 10:45:59 by svidot            #+#    #+#             */
/*   Updated: 2023/12/20 12:22:42 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"

void	set_filepaths(int *argc, char **argv[], char *filepaths[])
{
	filepaths[0] = *(++*argv);
	filepaths[1] = (*argv)[*argc - 2];
}

static void	close_fd(int fd_file[])
{
	if (fd_file[0] >= 0)
		close(fd_file[0]); // gerer -1
	if (fd_file[1] >= 0)	
		close(fd_file[1]); // gerer -1
}

static char	*create_strerror(char *error_str, char *filepath)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;
	
	tmp = ft_strjoin(strerror(errno), ": "); // gerer le NULL free error_str si aloué et close fd_file[0] si supp a -1
	tmp2 = ft_strjoin(tmp, filepath); // gerer le NULL: free s1 et error_str si aloué et fd_file[0] si supp a -1
	tmp3 = ft_strjoin(tmp2, "\n"); // gerer le NULL: free s1 et s2 et error_str si aloué et fd_file[0] si supp a -1
	tmp4 = NULL;
	if (*error_str)
		tmp4 = error_str;
	error_str = ft_strjoin(error_str, tmp3); // gerer le NULL: free s1 et s2 et s3 et s4 ou error_str si aloué et fd_file[0] si supp a -1
	free(tmp);
	free(tmp2);
	free(tmp3);
	if (tmp4)	
		free(tmp4);
	return (error_str);
}

void	get_fdio(int flag, char *filepaths[], int fd_file[])
{
	char	*error_str;
	
	error_str = "";
	if (!flag)
	{	
		fd_file[0] = open(filepaths[0], O_RDONLY);
		if (fd_file[0] < 0)		
			error_str = create_strerror(error_str, filepaths[0]);		
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_TRUNC, 400);
		if (fd_file[1] < 0)
			error_str = create_strerror(error_str, filepaths[1]);		
	}
	else
	{
		fd_file[0] = -1;
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_APPEND, 400);		
		if (fd_file[1] < 0)			
			error_str = create_strerror(error_str, filepaths[1]);				
	}
	if (*error_str)
		return (ft_putstr_fd(error_str, STDERR_FILENO), free(error_str),
			close_fd(fd_file), exit(EXIT_FAILURE));	// gerer ft_putstr_fd(write) et close
}
