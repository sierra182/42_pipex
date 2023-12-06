/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:14:52 by svidot            #+#    #+#             */
/*   Updated: 2023/12/06 14:09:21 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void assign_files(int argc, char *argv[], char *filepaths[])
{
	filespath[0] = argv[1];
	filespath[1] = argv[--argc];
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*filepaths[2];
	
	char	*cmd1;
	char	*cmd2;
	
	if (argc <= 4)
		return (1);
	assign_filepaths(argc, argv, filepaths);
	
	cmd1 = *++argv;
	
	cmd2 = *++argv;
	ft_printf("argv:%s\n", *(argv + 1));
	ft_printf("envp:%s\n", *envp);
	return (0);
}