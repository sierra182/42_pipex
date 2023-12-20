/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:23:23 by svidot            #+#    #+#             */
/*   Updated: 2023/12/20 13:06:44 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "ft_printf.h"
#include "libft.h"

#define BONUS 0
#ifdef EN_BONUS
# include "get_next_line.h"
# define BONUS 1
#endif

char	**parse_cmd(char *argv[], char *envp[], int fd_file[]);

void	set_filepaths(int *argc, char **argv[], char *filepaths[]);
void	get_fdio(int flag, char *filepaths[], int fd_file[]);

void	set_pipe_forward(int pipefd_in[], int pipefd_out[]) 
{			
	dup2(pipefd_in[0], STDIN_FILENO); // si -1 errno fd_file [1] pipefd_in[0][1] pipefd_out [0][1] cmds
	close(pipefd_in[0]); //  si -1 errno fd_file [1] pipefd_in[1] pipefd_out [0][1] cmds
	close(pipefd_in[1]); //  si -1 errno fd_file [1] pipefd_out [0][1] cmds
	dup2(pipefd_out[1], STDOUT_FILENO);	//  si -1 errno fd_file [1] pipefd_out [0][1] cmds
	close(pipefd_out[1]); //  si -1 errno fd_file [1] pipefd_out [0] cmds
	close(pipefd_out[0]);  //  si -1 errno fd_file [1] cmds
}

void	here_doc_handle(char **argv[], int pipefd_in[])
{
	char	*h_doc;
	char	*line;

	h_doc = **argv;
	while (1) // bof
	{
		ft_printf("heredoc> ");
		line = get_next_line(0);
		if (line)
		{			
			if (ft_strncmp(line, h_doc, ft_strlen(h_doc) - 1))
				ft_putstr_fd(line, pipefd_in[1]); // // gerer -1 errno close fd_file ?[0] [1] et free cmds et close pipefd_in ?[0] [1] et close pipefd_out [0] [1] et line et gnl(?)
			else
			{
				free(line);  
				get_next_line(42); // !!!
				break ; // sortie normale line et gnl(?)
			}
		}
		else
			exit(1); // sortie par eof anormale :gerer -1 errno close fd_file ?[0] [1] et free cmds et close pipefd_in ?[0] [1] et close pipefd_out [0] [1]
		free(line);
	}	
}

void	nurcery(char *argv[], char *envp[], int fd_file[], int	pipefd_in[2], int pipefd_out[2])
{
	pid_t 	pid;
	char 	**split;
	
	while(*(++argv + 1))
	{	
		pid = fork();	
		if (pid == 0)
		{		
			set_pipe_forward(pipefd_in, pipefd_out);		// si ok fd_file [1], cmds				
			split = parse_cmd(argv, envp, fd_file);			
			execve(split[0], split, envp);			
			exit(EXIT_FAILURE); // gerer errno -1, fd_file [1], cmds, et split	EXIT important sinon arbre!!!							 
		}
		else
		{			// gerer les processus fils en cours ? 
			close(pipefd_in[1]); //gerer -1 errno close fd_file [1] et free cmds et close pipefd_in [0] et close pipefd_out [0] [1]
			close(pipefd_in[0]); //gerer -1 errno close fd_file [1] et free cmds et close pipefd_out [0] [1]
			pipefd_in[0] = pipefd_out[0];
			pipefd_in[1] = pipefd_out[1];
			pipe(pipefd_out);	   		
		}
	}
}

void	create_pipeline(char *argv[], char *envp[], int fd_file[], int flag)
{	
	int		pipefd_in[2]; // pas oblg ?
	int		pipefd_out[2];
	char	buf;
	int 	status;
	
	pipe(pipefd_in); 
	pipe(pipefd_out);	
	if (!flag)
	{
		close(pipefd_in[0]); // gerer -1 errno close fd_file ?-1[0] [1] et free cmds et close pipefd_in [1] et close pipefd_out [0] [1] 
		pipefd_in[0] = fd_file[0];	
	}	
	if (flag)	
		here_doc_handle(&argv, pipefd_in); // si ok close fd_file [1] et free cmds et close pipefd_in [0] [1] et close pipefd_out [0] [1]
	nurcery(argv, envp, fd_file, pipefd_in, pipefd_out);
	close(pipefd_in[1]); //gerer -1 errno close fd_file [1] et close pipefd_in [0] et close pipefd_out [0] [1] et free cmds
	close(pipefd_out[1]); //gerer -1 errno close fd_file [1] et close pipefd_in [0] et close pipefd_out [0] et free cmds
	close(pipefd_out[0]); //gerer -1 errno close fd_file [1] et close pipefd_in [0] et free cmds
	while (read(pipefd_in[0], &buf, 1)) //gerer -1 errno close fd_file [1] et close pipefd_in [0] et free cmds
		ft_putchar_fd(buf, fd_file[1]); //gerer -1 errno close fd_file [1] et close pipefd_in [0] et free cmds
	close(pipefd_in[0]);  //gerer -1 errno close fd_file [1] et free cmds
	close(fd_file[1]); //et free cmds		
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*filepaths[2];
	int		fd_file[2];	
	int		flag;
	int 	status;

	flag = 0;	
	if (!ft_strcmp(*(argv + 1), "here_doc") && BONUS)
	{
		argv++;
		argc--; 
		flag = 1;
	}
	if (argc <= 4)
		return (1);
	set_filepaths(&argc, &argv, filepaths);
	get_fdio(flag, filepaths, fd_file);	
	create_pipeline(argv, envp, fd_file, flag);
	while (wait(&status) > 0) 
		;	
	return (0);
}
