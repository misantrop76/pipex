/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mminet <mminet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:14:57 by mminet            #+#    #+#             */
/*   Updated: 2024/04/15 14:33:11 by mminet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 20
# endif

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	int		status;
	pid_t	last_pid;
}			t_pipex;

char		*get_next_line(int fd);
int			open_file(char *path, int op, int *i);
void		usage(void);
char		**get_path(char **env);
void		free_tab(char **tab);

#endif
