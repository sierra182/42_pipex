/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:14:52 by svidot            #+#    #+#             */
/*   Updated: 2023/12/11 11:50:07 by svidot           ###   ########.fr       */
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

#include "get_next_line.h"

void	set_filepaths(int *argc, char **argv[], char *filepaths[])
{
	filepaths[0] = *(++*argv);
	filepaths[1] = (*argv)[--*argc - 1];
}

void	set_filepaths_hd(int argc, char **argv[], char *filepaths[])
{
	filepaths[0] = *(++*argv);
	filepaths[1] = (*argv)[argc - 3];
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

void	here_doc_handle(int *argc, char **argv[], int pipe_fd[])
{
	char	*h_doc;
	char	*line;

	h_doc = **argv;
	while (1)
	{
		line = get_next_line(0);
		if (line > 0)
		{			
			if (ft_strncmp(line, h_doc, ft_strlen(h_doc) - 1))
				ft_putstr_fd(line, pipe_fd[1]);
			else
				break ;
		}
	}
	close(pipe_fd[1]);
}

char	**parse_cmd(char *argv[], char *envp[])
{
	char ** split;


		split = ft_split(*argv, ' ');

	return split;
}

void	nurcery(int argc, char *argv[], char *envp[], int fd_file[], int flag)
{
	pid_t 	pid;
	int		pipefd_in[2];
	int		pipefd_out[2];
	char	buf;
	int 	status;
	
	if (pipe(pipefd_in) < 0)                              
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd_out) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	if (!flag)
		pipefd_in[0] = fd_file[0];
	if (flag)
		here_doc_handle(&argc, &argv, pipefd_in);

	ft_printf("argverrine %s, argc %d\n", *argv, argc);
	while(*(++argv + 1))
	{	
		pid = fork();		
		if (pid == 0)
		{	
			char **split = parse_cmd(argv, envp);		
			set_pipe_forward(pipefd_in, pipefd_out);							
			execve(split[0], split, envp);  
		}
		else
		{			
			close(pipefd_in[1]); 
			close(pipefd_in[0]);
			pipefd_in[0] = pipefd_out[0];
			pipefd_in[1] = pipefd_out[1];
			if (pipe(pipefd_out) < 0)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}     		
		}
	}
	//wait(&status);
	close(pipefd_in[1]);
	close(pipefd_out[1]);
	while (read(pipefd_in[0], &buf, 1))
		ft_putchar_fd(buf, fd_file[1]);
	close(pipefd_in[0]);	
}


void	nurcery2(int argc, char *argv[], char *envp[], int fd_file[])
{
	pid_t 	pid;
	int		pipefd_1[2];
	int		pipefd_2[2];
	int		pipefd_3[2];
	
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
	int		flag;

	flag = 0;
	if (!ft_strcmp(*(argv + 1), "here_doc"))
	{
		argv++;
		argc--;
		flag = 1;
	}
	if (argc <= 4)
		return (1);
	//if (!flag)
	set_filepaths(&argc, &argv, filepaths);
	//else
	//	set_filepaths_hd(argc, &argv, filepaths);
	ft_printf("argv:%s\n", *argv);
	ft_printf("filepath1:%s\n", filepaths[0]);
	ft_printf("filepath2:%s\n", filepaths[1]);	
	ft_printf("je suis dieu le pere et je vais me forker\n");
		
	if (!flag)
	{			
		fd_file[0] = open(filepaths[0], O_RDONLY);
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_TRUNC, 400);
	}	
	else
	{
		fd_file[0] = 0;
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_APPEND, 400);
	}
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
		nurcery(argc, argv, envp, fd_file, flag);
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
