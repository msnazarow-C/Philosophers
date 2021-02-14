/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 23:49:20 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 17:37:11 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "main.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int		getms(struct timeval start)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return ((tv.tv_sec - start.tv_sec) * 1000000 +
		(tv.tv_usec - start.tv_usec));
}

int		print_message(char *msg, t_phyl *phyl)
{
	sem_wait(phyl->arg->print);
	printf(msg, getms(phyl->arg->s_tv) / 1000, phyl->number + 1);
	sem_post(phyl->arg->print);
	return (0);
}

int		print_message_dead(char *msg, t_phyl *phyl)
{
	printf(msg, getms(phyl->arg->s_tv) / 1000, phyl->number + 1);
	return (0);
}

void	*take_forks(t_phyl *phyl)
{
	sem_wait(phyl->arg->forks);
	print_message("%ld %d has taken a fork\n", phyl);
	sem_wait(phyl->arg->forks);
	print_message("%ld %d has taken a fork\n", phyl);
	return (0);
}

void	*waitms(int us, t_arg *arg)
{
	int				dus;
	struct timeval	tv;
	struct timeval	sv;
	time_t			time;

	gettimeofday(&tv, 0);
	dus = 500;
	gettimeofday(&sv, 0);
	time = ((sv.tv_sec - tv.tv_sec) * 1000000 + (sv.tv_usec - tv.tv_usec));
	while (time < us && arg->state)
	{
		gettimeofday(&sv, 0);
		time = ((sv.tv_sec - tv.tv_sec) * 1000000 + (sv.tv_usec - tv.tv_usec));
		usleep(dus);
	}
	return (0);
}
