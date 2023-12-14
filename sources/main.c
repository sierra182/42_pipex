/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 11:14:52 by svidot            #+#    #+#             */
/*   Updated: 2023/12/14 18:58:20 by seblin           ###   ########.fr       */
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

void	here_doc_handle(int *argc, char **argv[], int pipefd_in[])
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
				ft_putstr_fd(line, pipefd_in[1]);
			else
				break ;
		}
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
	char	*temp;
	char	*temp2;
	
	temp = s;
	while (*arr)
	{
		temp2 = *arr;
		*arr = temp;
		temp = temp2;
		arr++;
	}
	*arr++ = temp;
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
		if (**split_arg == '\'')
		{			
			ft_delchar(*split_arg);
			start = split_arg;
		}
		if (*(*split_arg + ft_strlen(*split_arg) - 1) == '\'')
		{	
			ft_delchar(*split_arg + ft_strlen(*split_arg) - 1);			
			end = split_arg;		
		}		
		split_arg++;
	}
	if (start && end)
	{		
		char **temp;
		char *res;
		temp = start;
		res = *start;
		int j = 1;
		while (*++temp && *temp <= *end)
		{						
			res = ft_strjoin(ft_strjoin(res, " "), *temp);
			j++;
		}		
		split_arg = split_arg_save;
		while (*split_arg)
		{
			while (*split_arg == *start && j--)
			{			
				ft_delstr(split_arg);
			}			
			if (j == -1)
			{ 				
				ft_addstr(split_arg, res);
				break;
			}
			split_arg++;
		}	
	}	
}

void	join_simplequote2(char **split_arg)
{
	char	**start;
	char	**end;
	char	**split_arg_save;
	
	split_arg_save = split_arg;
	start = NULL;
	end = NULL;	
	while (*split_arg)
	{
		if (**split_arg == '\'')
		{			
			ft_delchar(*split_arg);
			start = split_arg;
		}
		if (*(*split_arg + ft_strlen(*split_arg) - 1) == '\'')
		{	
			ft_delchar(*split_arg + ft_strlen(*split_arg) - 1);			
			end = split_arg;		
		}		
		split_arg++;
	}
	if (start && end)
	{
		//ft_printf("we have champions, start : %s, end : %s\n", *start, *end);
		char **temp;
		char *res;
		temp = start;
		res = *start;
		int j = 1;
		while (*++temp && *temp <= *end)
		{			
			//ft_printf("we have a problem here: temp : %s\n", *temp);			
			res = ft_strjoin(ft_strjoin(res, " "), *temp);
			j++;
		}
		//ft_printf("bla, : -%s-\n", res);
		
		int i = 0;
		while (split_arg_save[i])
		{
			//ft_printf("split arg save, : -%s-\n", split_arg_save[i++]);			
		}
		//ft_printf("\n");
		split_arg = split_arg_save;
		while (*split_arg)
		{
			while (*split_arg == *start && j--)
			{
				//ft_printf("houhou -%s-\n", *split_arg);
				ft_delstr(split_arg);				
			
			}
				i = 0;
				while (split_arg[i])
				{
					//ft_printf("split arg lameme, : -%s-\n", split_arg[i++]);			
				}
			if (j == -1)
			{ //ft_printf("j egal zero\n");				
				i = 0;
				while (split_arg[i])
				{
					//ft_printf("split arg lamemeYEEE, : -%s-\n", split_arg[i++]);			
				}				
				ft_addstr(split_arg, res);
				break;
			}
			split_arg++;
		}
		i = 0;
		while (split_arg_save[i])
		{
			//ft_printf("split arg save, : -%s-\n", split_arg_save[i++]);			
		}
		//ft_printf("we have ONE champions, : %s\n", start);
	}	
}

char	**parse_cmd(char *argv[], char *envp[])
{
	char	**split_arg;
	char	**split_colon;
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
	join_simplequote(split_arg);
	if (env_find)
	{
		env_find += ft_strlen(env_to_find);
		split_colon = ft_split(env_find, ':');
		while (*split_colon)
		{
			cmd = ft_strjoin(ft_strjoin(*split_colon++, "/"), *split_arg);		
			if (!access(cmd, X_OK))
			{
				split_arg[0] = cmd;
				break;
			}
		}
		if (split_arg[0] != cmd)
			return (perror(split_arg[0]), exit(1), NULL);
	}
	return (split_arg);
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
	{
		close(pipefd_in[0]);
		pipefd_in[0] = fd_file[0];	
	}
	if (flag)	
		here_doc_handle(&argc, &argv, pipefd_in);		
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
	close(pipefd_in[1]);
	close(pipefd_out[1]);
	close(pipefd_out[0]);
	while (read(pipefd_in[0], &buf, 1))
		ft_putchar_fd(buf, fd_file[1]);
	close(pipefd_in[0]);
	close(fd_file[1]);
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

void	set_filepaths(int *argc, char **argv[], char *filepaths[])
{
	filepaths[0] = *(++*argv);
	filepaths[1] = (*argv)[--*argc - 1];
}

void	file_acces_handle2(int flag, char *filepaths[], int fd_file[])
{
	char	*error_str;
		//ft_printf("where is my seg fault\n");	
	error_str = "";
	if (!flag)
	{
		if (access(filepaths[0], F_OK))		
			error_str = ft_strjoin(error_str, ft_strjoin((ft_strjoin(ft_strjoin(strerror(errno), ": "), filepaths[0])), "\n"));
		
		else if (access(filepaths[0], R_OK))		
			error_str = ft_strjoin(error_str, ft_strjoin((ft_strjoin(ft_strjoin(strerror(errno), ": "), filepaths[0])), "\n"));
		
		fd_file[0] = open(filepaths[0], O_RDONLY);
		if (!access(filepaths[1], F_OK) && access(filepaths[1], W_OK))	
			error_str = ft_strjoin(error_str, ft_strjoin((ft_strjoin(ft_strjoin(strerror(errno), ": "), filepaths[1])), "\n"));
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_TRUNC, 400);
	}
	else
	{
		fd_file[0] = -1;
		if (!access(filepaths[1], F_OK) && access(filepaths[1], W_OK))	
			error_str = ft_strjoin(error_str, ft_strjoin((ft_strjoin(ft_strjoin(strerror(errno), ": "), filepaths[1])), "\n"));
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_APPEND, 400);
	}
	if (*error_str)
		return (ft_putstr_fd(error_str, STDERR_FILENO), exit(EXIT_FAILURE));		
}

void	free_error_str(char *s)
{
	free(s);
}

void	close_fd(int fd_file[])
{
	if (fd_file[0] >= 0)
		close(fd_file[0]);
	if (fd_file[1] >= 0)	
		close(fd_file[1]);
}

void	get_fdiotab(int flag, char *filepaths[], int fd_file[])
{
	char	*ptr[3][4];
	char *(*error_str)[4] = ptr; 
	char *(*error_str2)[4] = ptr; 
	int	i;
	int	j;
	
	//error_str = err_str;
	i = -1;
	while (++i < 3)
	{
		j = -1;		
		while (++j < 4)
		{		
			ptr[i][j] = NULL;
			error_str2[i][j] = NULL;
			error_str[i][j] = NULL;			
		}
	}
	//ptr[2] = NULL;
	if (!flag)
	{	
		fd_file[0] = open(filepaths[0], O_RDONLY);
		if (fd_file[0] < 0)	
		{
			(*error_str)[0] = ft_strdup(strerror(errno));
			(*error_str)[1] = ": ";
			(*error_str)[2] = filepaths[0];
			(*error_str++)[3] = "\n";				
		}	
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_TRUNC, 400);
		if (fd_file[1] < 0)			
		{
			(*error_str)[0] = ft_strdup(strerror(errno));
			(*error_str)[1] = ": ";
			(*error_str)[2] = filepaths[1];
			(*error_str++)[3] = "\n";	
					
		}	
	}
	else
	{
		fd_file[0] = -1;
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_APPEND, 400);		
		if (fd_file[1] < 0)			
		{
			(*error_str)[0] = ft_strdup(strerror(errno));
			(*error_str)[1] = ": ";
			(*error_str)[2] = filepaths[1];
			(*error_str)[3] = "\n";	
			//(*error_str)[4] = NULL;					
		}				
	}
	error_str = error_str2;

	while (*error_str[0])	
	{ft_printf("err: %s\n", **error_str);
		i = 0;
		while (i < 4)	
			ft_putstr_fd((*error_str)[i++], STDERR_FILENO);	
		error_str++;	
	
	}
	if (error_str2[0][0])
		free(error_str2[0][0]);
	if (error_str2[1][0])
		free(error_str2[1][0]);
	return (close_fd(fd_file), exit(EXIT_FAILURE));	
}


void	get_fdio(int flag, char *filepaths[], int fd_file[])
{
	char	*error_str;
	
	error_str = "";
	if (!flag)
	{	
		fd_file[0] = open(filepaths[0], O_RDONLY);
		if (fd_file[0] < 0)
		{
			char *s1 = ft_strjoin(strerror(errno), ": ");
			char *s2 = ft_strjoin(s1, filepaths[0]);
			char *s3 = ft_strjoin(s2, "\n");		
			error_str = ft_strjoin(error_str, s3);		
			free(s1);
			free(s2);
			free(s3);
			
		}	
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_TRUNC, 400);
		if (fd_file[1] < 0)			
		{
			char *s1 = ft_strjoin(strerror(errno), ": ");
			char *s2 = ft_strjoin(s1, filepaths[1]);
			char *s3 = ft_strjoin(s2, "\n");
			char *s4 = NULL;
			if (*error_str)
				s4 = error_str;
			error_str = ft_strjoin(error_str, s3);	
			free(s1);
			free(s2);
			free(s3);
			if (s4)	
				free(s4);
		}	
	}
	else
	{
		fd_file[0] = -1;
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_APPEND, 400);		
		if (fd_file[1] < 0)			
		{
			char *s1 = ft_strjoin(strerror(errno), ": ");
			char *s2 = ft_strjoin(s1, filepaths[1]);
			char *s3 = ft_strjoin(s2, "\n");			
			error_str = ft_strjoin(error_str, s3);	
			free(s1);
			free(s2);
			free(s3);				
		}				
	}
	if (*error_str)
		return (ft_putstr_fd(error_str, STDERR_FILENO), free_error_str(error_str), close_fd(fd_file), exit(EXIT_FAILURE));	
}

void	get_fdio2(int flag, char *filepaths[], int fd_file[])
{
	char	*error_str;
	
	error_str = "";
	if (!flag)
	{	
		fd_file[0] = open(filepaths[0], O_RDONLY);
		if (fd_file[0] < 0)			
			error_str = ft_strjoin(error_str, ft_strjoin(ft_strjoin(ft_strjoin(strerror(errno), ": "), filepaths[0]), "\n"));		
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_TRUNC, 400);
		if (fd_file[1] < 0)			
			error_str = ft_strjoin(error_str, ft_strjoin(ft_strjoin(ft_strjoin(strerror(errno), ": "), filepaths[1]), "\n"));	
	}
	else
	{
		fd_file[0] = -1;
		fd_file[1] = open(filepaths[1], O_WRONLY | O_CREAT | O_APPEND, 400);
		if (fd_file[1] < 0)		
			error_str = ft_strjoin(error_str, ft_strjoin(ft_strjoin(ft_strjoin(strerror(errno), ": "), filepaths[1]), "\n"));				
	}
	if (*error_str)
		return (ft_putstr_fd(error_str, STDERR_FILENO), free_error_str(error_str), close_fd(fd_file), exit(EXIT_FAILURE));	
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*filepaths[2];
	int		fd_file[2];	
	int		flag;

	flag = 0;
	if (!ft_strcmp(*(argv + 1), "here_doc"))
	{
		argv++;
		argc--; //
		flag = 1;
	}
	if (argc <= 4)
		return (1);
	set_filepaths(&argc, &argv, filepaths);
	get_fdio(flag, filepaths, fd_file);
	nurcery(argc, argv, envp, fd_file, flag);
	return (0);
}

int	main2(int argc, char *argv[], char *envp[])
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
	//ft_printf("argv:%s\n", *argv);
	//ft_printf("filepath1:%s\n", filepaths[0]);
	//ft_printf("filepath2:%s\n", filepaths[1]);	
	//ft_printf("je suis dieu le pere et je vais me forker\n");
		
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
