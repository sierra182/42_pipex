/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:14:52 by svidot            #+#    #+#             */
/*   Updated: 2023/12/06 16:04:24 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void	set_filepaths(int argc, char **argv[], char *filepaths[])
{
	filepaths[0] = *(++*argv);
	filepaths[1] = (*argv)++[argc - 2];
}

void	set_cmd(char **cmd_arr, char *argv[])
{
	int	n_cmd;
	
	n_cmd = -1;	
	while (*argv++)
		n_cmd++;
	//malloc
	ft_printf("filepath2:%d\n", n_cmd);
}

void	child_area()
{
	
}

void	parent_area()
{
	
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*filepaths[2];
	
	char	**cmd_arr;
	
	if (argc <= 4)
		return (1);
	set_filepaths(argc, &argv, filepaths);
	//set_cmd(cmd_arr, argv);
	ft_printf("argv:%s\n", *argv);
	ft_printf("filepath1:%s\n", filepaths[0]);
	ft_printf("filepath2:%s\n", filepaths[1]);	
	
	if (execve("/bin/ls", (char *[]) {"echo", "-l", NULL}, envp))
		perror("error execve");
	ft_printf("tu ne me verra jamais\n");
	// pid_t pid= fork();
	// if (pid < 0)
	// 	perror("error pid");
	// else if (pid == 0)
	// 	child_area();
	// else
	// 	parent_area();
	return (0);
}