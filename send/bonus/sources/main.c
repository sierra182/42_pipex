/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:23:23 by svidot            #+#    #+#             */
/*   Updated: 2024/01/06 15:42:54 by svidot           ###   ########.fr       */
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
#include "setup.h"
#include "bonus.h"

char	**parse_cmd(char *argv[], char *envp[], int fd_file[]);

void	set_pipe_forward(int pipefd_in[], int pipefd_out[])
{
	dup2(pipefd_in[0], STDIN_FILENO);
	close(pipefd_in[0]);
	close(pipefd_in[1]);
	dup2(pipefd_out[1], STDOUT_FILENO);
	close(pipefd_out[1]);
	close(pipefd_out[0]);
}

void	nurcery(char *argv[], char *envp[], int fd_file[], int *pipefd[])
{
	pid_t	pid;
	char	**split;

	while (*(++argv + 1))
	{
		pid = fork();
		if (pid == 0)
		{
			close(fd_file[1]);
			set_pipe_forward(pipefd[0], pipefd[1]);
			split = parse_cmd(argv, envp, fd_file);
			execve(split[0], split, envp);
			exit(EXIT_FAILURE);
		}
		else
		{
			close(pipefd[0][1]);
			close(pipefd[0][0]);
			pipefd[0][0] = pipefd[1][0];
			pipefd[0][1] = pipefd[1][1];
			pipe(pipefd[1]);
		}
	}
}
#ifdef EN_BONUS

void	here_doc_handle(char **argv[], int pipefd_in[])
{
	char	*h_doc;
	char	*line;

	h_doc = **argv;
	while (1)
	{
		ft_printf("heredoc> ");
		line = get_next_line(0);
		if (line)
		{
			line[ft_strlen(line) - 1] = 0;
			if (ft_strcmp(line, h_doc))
			{
				line[ft_strlen(line)] = '\n';
				ft_putstr_fd(line, pipefd_in[1]);
			}
			else
			{
				free(line);
				get_next_line(42);
				break ;
			}
		}
		free(line);
	}
}

#endif

void	create_pipeline(char *argv[], char *envp[], int fd_file[], int flag)
{
	int		pipefd_in[2];
	int		pipefd_out[2];
	char	buf;

	pipe(pipefd_in);
	pipe(pipefd_out);
	if (!flag)
	{
		close(pipefd_in[0]);
		pipefd_in[0] = fd_file[0];
	}
	else
		here_doc_handle(&argv, pipefd_in);
	nurcery(argv, envp, fd_file, (int *[]){pipefd_in, pipefd_out});
	close(pipefd_in[1]);
	close(pipefd_out[1]);
	close(pipefd_out[0]);
	while (read(pipefd_in[0], &buf, 1))
		ft_putchar_fd(buf, fd_file[1]);
	close(pipefd_in[0]);
	close(fd_file[1]);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*filepaths[2];
	int		fd_file[2];
	int		flag;

	flag = 0;
	if (argc == 1)
		return (1);
	if (!ft_strcmp(*(argv + 1), "here_doc") && BONUS)
	{
		argv++;
		argc--;
		flag = 1;
	}
	if ((argc != 5 && !BONUS) || (argc <= 4 && BONUS))
		return (1);
	set_filepaths(&argc, &argv, filepaths);
	get_fdio(flag, filepaths, fd_file);
	create_pipeline(argv, envp, fd_file, flag);
	while (wait(&(int){0}) > 0)
		;
	return (0);
}
