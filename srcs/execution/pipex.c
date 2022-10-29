/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gafreita <gafreita@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 22:45:57 by gafreita          #+#    #+#             */
/*   Updated: 2022/10/29 14:08:00 by gafreita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	auxiliar function that closes all pipes */
static void	close_pipes(int *pipes)
{
	int	i;
	int	pipe_nb;

	pipe_nb = (base()->num_pipes - 1) * 2;
	i = 0;
	while (i < pipe_nb)
	{
		close(pipes[i]);
		i++;
	}
}

/*	initializes one pipe for each | on input
	there are 2 enterings for each pipe,
	so we have to alloc 2 * number of pipes
*/
static int	*pipe_ini(void)
{
	int	*pipes;
	int	i;

	pipes = malloc(sizeof(int) * (base()->num_pipes - 1) * 2);
	i = 0;
	while (i < (base()->num_pipes - 1) * 2)
	{
		pipe(pipes + i);
		i += 2;
	}
	return (pipes);
}

/*	executes the command in the position cmd, reading from stdin_fd and
	writing in stdout_fd if this fd are -1 it reads and writes on
	STDIN_FILENO, STDOUT_FILENO all pipes are closed to prevent the
	program to be waiting forever
*/
static void	exec_setup(int stdin_fd, int stdout_fd, int cmd, int *pipes)
{
	open_files(&base()->pipes[cmd]);
	/* DUP FILE DESCRIPTORS
		EITHER THE PIPE OR THE FILE FROM REDIRECTION */
	if (stdout_fd > 1 && base()->pipes[cmd].fd[OUT] == STD)
		dup2(stdout_fd, STDOUT_FILENO);
	else
		dup2(base()->pipes[cmd].fd[OUT], STDOUT_FILENO);
	if (stdin_fd > 1 && base()->pipes[cmd].fd[IN] == STD)
		dup2(stdin_fd, STDIN_FILENO);
	else
		dup2(base()->pipes[cmd].fd[IN], STDIN_FILENO);
	/* DUP DONE */
	close_pipes(pipes);
	if (base()->pipes[cmd].heredoc)
		close(((int *)base()->pipes[cmd].heredoc)[0]);
	execute(base()->pipes[cmd].cmds, -1);
	exit (base()->errnumb);
}

static void	wait_aux(int *status)
{
	int	i;

	i = -1;
	while (++i < (base()->num_pipes))
	{
		wait(status);
		base()->errnumb = WEXITSTATUS(*status);
	}
}

/*	executes a list of commands delimited by pipes */
void	loop_pipex(void)
{
	int	status;
	int	i;
	int	*pipes;

	pipes = pipe_ini();
	if (fork() == 0)
		exec_setup(STD, pipes[1], 0, pipes);
	else
	{
		signal(SIGINT, sig_block);
		i = 1;
		while (i < (base()->num_pipes) - 1)
		{
			if (fork() == 0)
				exec_setup(pipes[(i - 1) * 2], pipes[(i - 1) * 2 + 3], i, pipes);
			else
				i++;
		}
	}
	if (fork() == 0)
		exec_setup(pipes[(i - 1) * 2], STD, i, pipes);
	close_pipes(pipes);
	wait_aux(&status);
	free(pipes);
}
