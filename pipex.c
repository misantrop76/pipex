/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:35:38 by mminet            #+#    #+#             */
/*   Updated: 2024/04/12 17:38:44 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

void	free_tab(char **tab)
{
	int i;

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

void	my_exec(char *arg, char **env)
{
	char	**path;
	char	*test;
	char	**args;
	int		i;

	path = get_path(env);
	args = ft_split(arg, ' ');
	i = 0;
	execve(args[0], args, env);
	while (path[i])
	{
		test = ft_strjoin(path[i], args[0]);
		dprintf(2, "%s\n", test);
		execve(test, args, env);
		free(test);
		i++;
	}
	perror(strerror(errno));
	exit(EXIT_FAILURE);
}

void	ft_pipe_loop(t_pipex *pipex, int *pipefd[pipex->nb_cmd][2])
{
	int i;
	pid_t pid;

	i = 0;
	(void)pipefd;
	while (i < pipex->nb_cmd)
	{
		pid = fork();
		if (pid == -1)
		{
			perror(strerror(errno));
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (i > 0)
			{
				dup2(*pipefd[i - 1][0], STDIN_FILENO);
				dprintf(2, "je prend depuis le pipe\n");
				close(*pipefd[i - 1][1]);
			}
			else
			{
				dup2(pipex->fd_in, STDIN_FILENO);
				dprintf(2, "je prend depuis le fichier\n");
			}
			if (i < pipex->nb_cmd - 1)
			{
				dprintf(2, "je redirige vers le pipe\n");
				dup2(pipefd[i][1], STDOUT_FILENO);
				close(pipefd[i][0]);
			}
			else
			{
				dup2(pipex->fd_out, STDOUT_FILENO);
				dprintf(2, "je met dans fichier\n");
			}
			my_exec(pipex->av[i + 2], pipex->env);
		}
		wait(NULL);
		i++;
	}
}

void	ft_pipe(t_pipex *pipex)
{
	int 	i;
	int		pipefd[pipex->nb_cmd - 1][2];

	i = 0;
	while (i < pipex->nb_cmd)
	{
		if (pipe(pipefd[i]) == -1)
		{
			perror(strerror(errno));
			exit(EXIT_FAILURE);
		}
		i++;
	}
	ft_pipe_loop(pipex, &pipefd);
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	pipex.av = av;
	pipex.env = env;
	pipex.nb_cmd = ac - 3;
	if (ac < 4)
		return (0);
	if (access(av[1], R_OK) != 0)
	{
		ft_putstr_fd(av[1], 1);
		ft_putstr_fd(": Aucun fichier ou dossier de ce nom\n", 1);
	}
	pipex.fd_in = open(av[1], O_RDONLY);
	pipex.fd_out = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 00664);
	ft_pipe(&pipex);
}
