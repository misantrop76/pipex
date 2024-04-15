/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 19:24:59 by mminet            #+#    #+#             */
/*   Updated: 2024/04/15 18:29:54 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

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
		execve(test, args, env);
		free(test);
		i++;
	}
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd("\n", 2);
	free_tab(path);
	free_tab(args);
	exit(127);
}

void	do_pipe(char *cmd, char **env, t_pipex *pipex, int i)
{
	pid_t	pid;
	int		p_fd[2];

	if (pipe(p_fd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (!pid)
	{
		close(p_fd[0]);
		if (i)
			dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
		my_exec(cmd, env);
	}
	else
	{
		pipex->last_pid = pid;
		close(p_fd[1]);
		if (i)
			dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
	}
}

int	main(int ac, char **av, char **env)
{
	int		i;
	t_pipex	pipex;

	if (ac != 5)
		usage();
	i = 2;
	pipex.fd_out = open_file(av[ac - 1], 1, &i);
	pipex.fd_in = open_file(av[1], 2, &i);
	dup2(pipex.fd_in, STDIN_FILENO);
	if (i == 2)
		do_pipe(av[i], env, &pipex, 1);
	if (access(av[ac - 1], W_OK))
		exit(EXIT_FAILURE);
	dup2(pipex.fd_out, STDOUT_FILENO);
	do_pipe(av[ac - 2], env, &pipex, 0);
	waitpid(pipex.last_pid, &pipex.status, 0);
	//wait(NULL);
	close(pipex.fd_in);
	close(pipex.fd_out);
	if (pipex.status)
		exit(pipex.status >> 8);
	exit(EXIT_SUCCESS);
}
