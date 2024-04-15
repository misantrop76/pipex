/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:26:41 by mminet            #+#    #+#             */
/*   Updated: 2024/04/15 18:32:36 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**get_path(char **env)
{
	int		i;
	char	**path;
	char	*tmp;

	i = 0;
	while (env[i] != NULL && ft_strncmp(env[i], "PATH=", 4) != 0)
		i++;
	if (env[i] == NULL)
		return (NULL);
	path = ft_split(env[i] + 5, ':');
	i = 0;
	while (path[i])
	{
		tmp = path[i];
		path[i] = ft_strjoin(path[i], "/");
		free(tmp);
		i++;
	}
	return (path);
}

void	print_error(int i, char *path)
{
	if (i)
	{
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Aucun fichier ou dossier de ce nom\n", 2);
	}
	else
	{
		ft_putstr_fd("permission denied: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd("\n", 2);
	}
}

void	usage(void)
{
	ft_putstr_fd("Error\n", STDOUT_FILENO);
	ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2\n", STDOUT_FILENO);
	exit(EXIT_FAILURE);
}

int	open_file(char *path, int op, int *i)
{
	int	fd;

	if (op == 0 || op == 1)
	{
		if (i == 0)
			fd = open(path, O_CREAT | O_RDWR | O_APPEND, 00664);
		else
			fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 00664);
		if (access(path, W_OK))
			print_error(0, path);
	}
	else
	{
		if (access(path, R_OK) == 0)
			fd = open(path, O_RDONLY, 0664);
		else
		{
			print_error(1, path);
			fd = open("/dev/null", O_RDONLY);
			*i = 3;
		}
	}
	return (fd);
}
