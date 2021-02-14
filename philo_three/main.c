/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 10:22:58 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 19:54:36 by sgertrud         ###   ########.fr       */
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
#include <sys/types.h>
#include <signal.h>

void	*dead_exit(sem_t *kill_, int n, pid_t *pids)
{
	int		i;

	sem_wait(kill_);
	i = -1;
	while (++i < n)
		kill(pids[i], SIGKILL);
	exit(0);
	return (0);
}

void	*order_check(void *arg)
{
	int		i;
	t_phyl	*phyls;

	phyls = (t_phyl*)arg;
	i = -1;
	while (++i < phyls->arg->number)
	{
		sem_post(phyls[i].start);
		sem_wait(phyls[i].end);
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

void	*eat_check(void *arg)
{
	int		i;
	t_phyl	*phyls;

	phyls = (t_phyl*)arg;
	i = -1;
	while (++i < phyls->arg->number)
		sem_wait(phyls[i].sync);
	sem_wait(phyls->arg->print);
	printf("All philosophers are alive!\n");
	sem_post(phyls->arg->kill_);
	return (0);
}

int		main(int argc, char const *argv[])
{
	int					number;
	t_phyl				*phyls;
	pid_t				*pids;

	if (argc == 5 || argc == 6)
	{
		number = ft_atoi(argv[1]);
		phyls = malloc(number * sizeof(t_phyl));
		phyls->arg = malloc(sizeof(t_arg));
		phyls->arg->number = number;
		parse(phyls, argv);
		pids = malloc(number * sizeof(pid_t));
		init(phyls, pids);
		pthread_create(&phyls->arg->order, NULL, order_check, phyls);
		pthread_create(&phyls->arg->food, NULL, eat_check, phyls);
		dead_exit(phyls->arg->kill_, number, pids);
		sem_wait(phyls->arg->print);
		printf("All philosophers are alive!\n");
		return (0);
	}
}
