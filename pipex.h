/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:14:57 by mminet            #+#    #+#             */
/*   Updated: 2024/04/12 13:38:16 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	char	**env;
	int		*pipefd[2];
	char	**path;
	int		nb_cmd;
	char	**av;
}				t_pipex;

#endif
