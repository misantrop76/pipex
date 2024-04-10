/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 20:35:38 by mminet            #+#    #+#             */
/*   Updated: 2024/04/11 01:31:45 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

int main(int ac, char **av, char **env)
{
    int fd;
    int fd_out;
    int pipefd[2];
    char    **args = ft_split(av[2], ' ');
    pid_t pid;

    pipe(pipefd);
    if (ac < 5)
        return (0);
    if (access(av[1], R_OK) != 0)
    {
        ft_putstr_fd(av[1], 1);
        ft_putstr_fd(": Aucun fichier ou dossier de ce nom\n", 1);
    }
    fd_out = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 00664);
    fd = open(av[1], O_RDONLY);
    pid = fork();
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(fd, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        execve(ft_strjoin("/usr/bin/", args[0]), args, env);
    }
    else
    {
        wait(NULL);
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        dup2(fd_out, STDOUT_FILENO);
        args = ft_split(av[3], ' ');
        execve(ft_strjoin("/usr/bin/", args[0]), args, env);
    }
}