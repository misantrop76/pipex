/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:35:38 by mminet            #+#    #+#             */
/*   Updated: 2024/04/14 20:22:16 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	put_here_doc(char **av, int *p_fd)
{
	char	*ret;

	close(p_fd[0]);
	while (1)
	{
		write(1, ">", 1);
		ret = get_next_line(0);
		if (ft_strncmp(ret, av[2], ft_strlen(av[2]) - 1) == 0)
		{
			free(ret);
			exit(EXIT_SUCCESS);
		}
		ft_putstr_fd(ret, p_fd[1]);
		free(ret);
	}
}

void	here_doc(char **av)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		put_here_doc(av, fd);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
}

void	do_pipe(char *cmd, char **env, int *status)
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
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
		my_exec(cmd, env);
	}
	else
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		waitpid(pid, status, 0);
	}
}

int	main(int ac, char **av, char **env)
{
	int		i;
	t_pipex pipex;

	if (ac < 5 || (ac < 6 && ft_strncmp(av[1], "here_doc", 8) == 0))
		usage();
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		i = 3;
		pipex.fd_out = open_file(av[ac - 1], 0, &i);
		here_doc(av);
	}
	else
	{
		i = 2;
		pipex.fd_out = open_file(av[ac - 1], 1, &i);
		pipex.fd_in = open_file(av[1], 2, &i);
		dup2(pipex.fd_in, STDIN_FILENO);
	}
	while (i < ac - 2)
		do_pipe(av[i++], env, &pipex.status);
	dup2(pipex.fd_out, STDOUT_FILENO);
	my_exec(av[ac - 2], env);
	exit(pipex.status);
	close(pipex.fd_in);
	close(pipex.fd_out);
}
