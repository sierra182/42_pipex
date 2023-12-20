/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:23:23 by svidot            #+#    #+#             */
/*   Updated: 2023/12/20 09:10:18 by seblin           ###   ########.fr       */
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
		ft_printf("here_doc ");
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
			exit(1);ft_printf("fin "); // sortie par eof anormale :gerer -1 errno close fd_file ?[0] [1] et free cmds et close pipefd_in ?[0] [1] et close pipefd_out [0] [1]
		free(line);
	}	
}

void	ft_delstr(char **arr)
{
	while (*arr)
	{
		*arr = *(arr + 1);
		arr++;
	}	
}

void	ft_addstr(char **arr, char *s)
{
	char	*tmp;
	char	*tmp2;
	
	tmp = s;
	while (*arr)
	{
		tmp2 = *arr;
		*arr = tmp;
		tmp = tmp2;
		arr++;
	}
	*arr++ = tmp;
	arr = 0;
}
void	join_simplequote(char **split_arg)
{
	char	**start;
	char	**end;
	char	**split_arg_save;
	
	split_arg_save = split_arg;
	start = NULL;
	end = NULL;	
	while (*split_arg)
	{
		char *split_arg_save2 = *split_arg;
		while (*(*split_arg + 1))
		{			
			if (**split_arg == '\'' || **split_arg == '\"')
			{			
				ft_delchar(*split_arg);
				start = split_arg;
			}
			(*split_arg)++;
		}
		*split_arg = split_arg_save2;
		if (*(*split_arg + ft_strlen(*split_arg) - 1) == '\'' || *(*split_arg + ft_strlen(*split_arg) - 1) == '\"')
		{	
			ft_delchar(*split_arg + ft_strlen(*split_arg) - 1);			
			end = split_arg;		
		}		
		split_arg++;
	}
	if (start && end && start != end)
	{		ft_putstr_fd("yaaaa\n", 2);
		char **temp;
		char *res;
		temp = start;
		res = *start;
		int j = 1;
		while (*++temp && *temp <= *end)
		{						
			res = ft_strjoin(ft_strjoin(res, " "), *temp);
			j++;ft_putstr_fd("yuuu\n", 2);
		}		
		split_arg = split_arg_save;
		while (*split_arg)
		{ft_putstr_fd("yiii\n", 2);
			while (*split_arg == *start && j--)
			{			
				ft_delstr(split_arg); ft_putstr_fd("yeee\n", 2);
			}			
			if (j == -1)
			{ 				
				ft_addstr(split_arg, res); ft_putstr_fd("yooo\n", 2);
				break;
			}
			split_arg++;
		}	
	}	
}

char	**parse_cmd(char *argv[], char *envp[], int fd_file[])
{
	char	**split_arg;
	char	**split_colon;
	char	**split_colon_sav;
	char	*env_to_find;
	char	*env_find;
	char	*cmd;
	
	env_to_find = "PATH=";
	env_find = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp++, env_to_find, ft_strlen(env_to_find)))
		{
			env_find = *--envp;			
			break ;
		}
	}
	if (!env_find)
		return (perror("env Path not found"), exit(1), NULL);
	split_arg = ft_split(*argv, ' ');	
	//join_simplequote(split_arg);	
	env_find += ft_strlen(env_to_find);
	split_colon = ft_split(env_find, ':');
	split_colon_sav = split_colon;
	while (*split_colon)
	{
		char	*s1;

		s1 = ft_strjoin(*split_colon++, "/");
		cmd = ft_strjoin(s1, *split_arg);
		free(s1);	
		if (!access(cmd, X_OK))
		{
			free(*split_arg);
			*split_arg = cmd;
			break;
		}
		free(cmd);
		cmd = NULL;
	}
	if (!cmd)
	{
		close(fd_file[1]);
		int i;
		i = 0;
		while (split_colon_sav[i])
			free(split_colon_sav[i++]);
		free(split_colon_sav);
		perror(*split_arg);
		i = 0;
		while (split_arg[i])
			free(split_arg[i++]);
		free(split_arg);	
		return (exit(1), NULL);
	}	
	return (split_arg);
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

void	create_pipeline(char *argv[], char *envp[], int fd_file[], int flag) // pas oblg ?
{	
	int		pipefd_in[2];
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

void	set_filepaths(int *argc, char **argv[], char *filepaths[])
{
	filepaths[0] = *(++*argv);
	filepaths[1] = (*argv)[*argc - 2];
}

void	close_fd(int fd_file[])
{
	if (fd_file[0] >= 0)
		close(fd_file[0]); // gerer -1
	if (fd_file[1] >= 0)	
		close(fd_file[1]); // gerer -1
}
char	*create_strerror(char *error_str, char *filepaths[])
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*tmp4;
	
	tmp = ft_strjoin(strerror(errno), ": "); // gerer le NULL free error_str si aloué et close fd_file[0] si supp a -1
	tmp2 = ft_strjoin(tmp, filepaths[1]); // gerer le NULL: free s1 et error_str si aloué et fd_file[0] si supp a -1
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
			error_str = create_strerror(error_str, filepaths);		
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_TRUNC, 400);
		if (fd_file[1] < 0)
			error_str = create_strerror(error_str, filepaths);		
	}
	else
	{
		fd_file[0] = -1;
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_APPEND, 400);		
		if (fd_file[1] < 0)			
			error_str = create_strerror(error_str, filepaths);				
	}
	if (*error_str)
		return (ft_putstr_fd(error_str, STDERR_FILENO), free(error_str),
			close_fd(fd_file), exit(EXIT_FAILURE));	// gerer ft_putstr_fd(write) et close
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*filepaths[2];
	int		fd_file[2];	
	int		flag;
	int 	status;
	
	flag = 0;
	if (!ft_strcmp(*(argv + 1), "here_doc"))
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





