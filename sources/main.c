/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:14:52 by svidot            #+#    #+#             */
/*   Updated: 2023/12/09 16:33:25 by svidot           ###   ########.fr       */
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
	filepaths[1] = (*argv)[argc - 2];
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

void	set_pipe_forward(int pipefd_in[], int pipefd_out[])
{
			
	dup2(pipefd_in[0], STDIN_FILENO);
	close(pipefd_in[0]);
	close(pipefd_in[1]);

	dup2(pipefd_out[1], STDOUT_FILENO);	
	close(pipefd_out[1]);
	close(pipefd_out[0]);
}

void	nurcery(int argc, char *argv[], char *envp[], int fd_file[])
{
	pid_t 	pid;
	int		pipefd_1[2];
	int		pipefd_2[2];
	int		pipefd_3[2];
	int		status;	
	
	if (pipe(pipefd_1) < 0)                              // 1er pipe
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd_2) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd_3) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	//dup2( fd_file[0], )
	pipefd_1[0] = fd_file[0];
	//ft_putstr_fd("la reponse est la quelque part, elle te cherche aussi\n", pipefd_1[1]);
	//close(pipefd_1[1]);
	pid = fork();		
	if (pid == 0)
	{	
		close(pipefd_3[0]);	
		close(pipefd_3[1]);					// enf 1
		set_pipe_forward(pipefd_1, pipefd_2);							// 2eme pipe
		execve("/bin/rev", (char *[]) {"rev", NULL}, envp);  
	}
	else
	{
		close(pipefd_1[0]);
		close(pipefd_2[1]);	                             // parent
		pid = fork();
		if (pid == 0)								// enf 2
		{
			close(pipefd_1[0]);
			close(pipefd_1[1]);	
			ft_putstr_fd("LA2\n", 2);
			set_pipe_forward(pipefd_2, fd_file);			// 3eme pipe
			execve("/bin/rev", (char *[]) {"rev", NULL}, envp); 
		}
		else
		{											 // parent
			char buf;
			close(pipefd_2[0]);
			close(pipefd_3[1]);		
			while (read(pipefd_3[0], &buf, 1))
				ft_putchar_fd(buf, 1);
			close(pipefd_3[0]);			
		}	
	}
}
void	child_area(pid_t pid, int pipefd[], char *envp[])
{	
	ft_printf("je suis ton fils, mon nom est: %d\n", pid);
	ft_putstr_fd("la reponse est la quelque part, elle te cherche aussi\n", pipefd[1]);
	
	dup2(pipefd[0], STDIN_FILENO);
	//dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	//close(pipefd[1]);
	if (execve("/bin/cat", (char *[]) {"rev", NULL}, envp))
		perror("error execve");
	ft_printf("je suis ta soeur: %d\n", pid);
}

void	parent_area(pid_t pid, int pipefd[])
{
	char	buf;
	
	ft_printf("je suis ton pere, ton nom est: %d\n", pid);
	
	// while (read(pipefd[0], &buf, 1))
	// ft_putchar_fd(buf, 1);
	// close(pipefd[0]);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*filepaths[2];
	int		fd_file[2];
	char	**cmd_arr;
	int		pipefd[2];
		
	if (argc <= 4)
		return (1);
	set_filepaths(argc, &argv, filepaths);
	ft_printf("argv:%s\n", *argv);
	ft_printf("filepath1:%s\n", filepaths[0]);
	ft_printf("filepath2:%s\n", filepaths[1]);	
	ft_printf("je suis dieu le pere et je vais me forker\n");
	fd_file[0] = open(filepaths[0], O_RDONLY);
	fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT, 400);
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
		nurcery(argc, argv, envp, fd_file);
		//child_area(pid, pipefd, envp);
	else
	{
		int	status;	
		close(pipefd[1]);
		close(pipefd[0]);	
		wait(&status);
		//parent_area(pid, pipefd);
	}
	return (0);
}
