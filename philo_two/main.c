/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 10:22:58 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 19:21:45 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include "main.h"

void	*order_check(void *arg)
{
	int		i;
	t_phyl	*phyls;

	phyls = (t_phyl*)arg;
	gettimeofday(&phyls->arg->s_tv, 0);
	i = -1;
	while (++i < phyls->arg->number && (phyls->arg->hungry > 0))
	{
		phyls[i].eat = phyls->arg->s_tv;
		sem_post(phyls[i].start);
	}
	while (phyls->arg->state && (phyls->arg->hungry > 0))
	{
		i = -1;
		while (++i < phyls->arg->number && (phyls->arg->hungry > 0))
		{
			sem_post(phyls[i].permf);
			sem_wait(phyls[i].conf);
		}
	}
	return (0);
}

void	*free_exit(t_phyl *phyls)
{
	int i;

	phyls->arg->state = 1;
	i = -1;
	while (++i < phyls->arg->number)
	{
		sem_post(phyls[i].permf);
		sem_post(phyls[i].conf);
		sem_post(phyls[i].dead);
		pthread_join(phyls[i].phylo, 0);
	}
	pthread_join(phyls->arg->order, 0);
	free(phyls->arg);
	free(phyls);
	return (0);
}

int		main(int argc, char const *argv[])
{
	int					number;
	t_phyl				*phyls;

	if (argc == 5 || argc == 6)
	{
		number = ft_atoi(argv[1]);
		phyls = malloc(number * sizeof(t_phyl));
		phyls->arg = malloc(sizeof(t_arg));
		phyls->arg->number = number;
		parse(phyls, argv);
		phyls->arg->print = sem_open("print", O_CREAT | O_EXCL, 0644, 1);
		sem_unlink("print");
		phyls->arg->forks = sem_open("forks", O_CREAT | O_EXCL, 0644, number);
		sem_unlink("forks");
		phyls->arg->sync = sem_open("sync", O_CREAT | O_EXCL, 0644, 1);
		sem_unlink("sync");
		init(phyls);
		order_check(phyls);
		sem_wait(phyls->arg->print);
		printf("All philosophers are alive!\n");
		return (0);
	}
}
