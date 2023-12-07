/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:14:52 by svidot            #+#    #+#             */
/*   Updated: 2023/12/07 19:32:58 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

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

void	child_area(pid_t pid, int pipefd[], char *envp[])
{
	ft_printf("je suis ton fils, mon nom est: %d\n", pid);
	ft_putstr_fd("la reponse est la quelque part, elle te cherche aussi\n", pipefd[1]);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	if (execve("/bin/rev", (char *[]) {"rev", NULL}, envp))
		perror("error execve");
	ft_printf("je suis ta soeur: %d\n", pid);
}

void	parent_area(pid_t pid, int pipefd[])
{
	char	buf;
	
	ft_printf("je suis ton pere, ton nom est: %d\n", pid);
	
	// while (read(pipefd[0], &buf, 1))	
	// 	ft_putchar_fd(buf, 1);		
	// close(pipefd[0]);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*filepaths[2];	
	char	**cmd_arr;
	int		pipefd[2];
	
	
	if (argc <= 4)
		return (1);
	set_filepaths(argc, &argv, filepaths);
	ft_printf("argv:%s\n", *argv);
	ft_printf("filepath1:%s\n", filepaths[0]);
	ft_printf("filepath2:%s\n", filepaths[1]);	
	
	ft_printf("je suis le pere et je vais me forker la face\n");
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);	
	}
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);		
	}
	else if (pid == 0)
		child_area(pid, pipefd, envp);
	else
	{
		int	status;	
		close(pipefd[1]);
		close(pipefd[0]);	
		wait(&status);
		parent_area(pid, pipefd);
	}
	return (0);
}
