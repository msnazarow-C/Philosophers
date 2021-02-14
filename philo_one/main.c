/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/20 10:22:58 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 16:36:47 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include "main.h"

void	*order_check(void *arg)
{
	int		i;
	t_phyl	*phyls;

	phyls = (t_phyl*)arg;
	gettimeofday(&phyls->arg->s_tv, 0);
	i = -1;
	while (++i < phyls->arg->number && (phyls[i].eat = phyls->arg->s_tv).tv_sec)
		pthread_mutex_unlock(&phyls[i].start);
	while (phyls->arg->state && (phyls->arg->hungry > 0))
	{
		i = -2;
		while ((i += 2) < phyls->arg->number && (phyls->arg->hungry > 0))
		{
			pthread_mutex_unlock(&phyls[i].permf);
			pthread_mutex_lock(&phyls[i].conf);
		}
		i = -1;
		while ((i += 2) < phyls->arg->number && (phyls->arg->hungry > 0))
		{
			pthread_mutex_unlock(&phyls[i].permf);
			pthread_mutex_lock(&phyls[i].conf);
		}
	}
	return (0);
}

void	*free_exit(t_phyl *phyls, pthread_mutex_t *forks)
{
	int i;

	phyls->arg->state = 1;
	i = -1;
	while (++i < phyls->arg->number)
	{
		pthread_mutex_unlock(&phyls[i].permf);
		pthread_mutex_unlock(&phyls[i].conf);
		pthread_mutex_unlock(phyls[i].lfork);
		pthread_mutex_unlock(phyls[i].rfork);
		pthread_mutex_unlock(&phyls[i].dead);
		pthread_join(phyls[i].phylo, 0);
	}
	pthread_join(phyls->arg->order, 0);
	free(phyls->arg);
	free(phyls);
	free(forks);
	return (0);
}

int		main(int argc, char const *argv[])
{
	pthread_mutex_t		*forks;
	int					number;
	t_phyl				*phyls;

	if (argc == 5 || argc == 6)
	{
		number = ft_atoi(argv[1]);
		phyls = malloc(number * sizeof(t_phyl));
		phyls->arg = malloc(sizeof(t_phyl));
		phyls->arg->number = number;
		parse(phyls, argv);
		pthread_mutex_init(&phyls->arg->print, 0);
		pthread_mutex_init(&phyls->arg->sync, 0);
		forks = malloc(phyls->arg->number * sizeof(pthread_mutex_t));
		init(phyls, forks);
		order_check(phyls);
		printf("All philosophers are alive!\n");
	}
	return (0);
}
