/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 10:17:43 by svidot            #+#    #+#             */
/*   Updated: 2024/01/03 10:03:10 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "libft.h"

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
		return (perror("env Path not found"), close(fd_file[1]), exit(1), NULL);
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
		perror(*split_arg); // command not found
		i = 0;
		while (split_arg[i])
			free(split_arg[i++]);
		free(split_arg);	
		return (exit(1), NULL);
	}	
	return (split_arg);
}