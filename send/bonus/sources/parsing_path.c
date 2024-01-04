/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:06:02 by svidot            #+#    #+#             */
/*   Updated: 2024/01/04 14:56:00 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "libft.h"
#include "parsing_build.h"

static char	*try_paths(char **split_arg, char *env_find)
{
	char	*cmd;
	char	**split_colon;
	char	**split_colon_sav;
	char	*tmp;

	cmd = NULL;
	split_colon = ft_split(env_find, ':');
	split_colon_sav = split_colon;
	while (split_colon && *split_colon)
	{
		tmp = ft_strjoin(*split_colon++, "/");
		cmd = ft_strjoin(tmp, *split_arg);
		free(tmp);
		if (!access(cmd, X_OK))
		{
			free(*split_arg);
			*split_arg = cmd;
			break ;
		}
		free(cmd);
		cmd = NULL;
	}
	free_ptr_arr(split_colon_sav);
	return (cmd);
}

static char	*search_path(char *envp[])
{
	char	*env_to_find;
	char	*env_find;

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
		return (NULL);
	env_find += ft_strlen(env_to_find);
	return (env_find);
}

char	**parse_cmd(char *argv[], char *envp[], int fd_file[])
{
	char	**split_arg;
	char	*env_find;

	env_find = search_path(envp);
	if (!env_find)
	{
		ft_putstr_fd("env PATH not found.\n", 2);
		close(fd_file[1]);
		exit(1);
	}
	split_arg = create_ast(*argv, fd_file);
	if (!try_paths(split_arg, env_find))
	{
		close(fd_file[1]);
		perror(*split_arg);
		free_ptr_arr(split_arg);
		exit(1);
	}
	return (split_arg);
}
