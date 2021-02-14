/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 00:13:18 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 17:35:09 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <stdlib.h>
#include <unistd.h>

void	sync_hungry(t_phyl *phyl)
{
	phyl->dinners--;
	if (!phyl->dinners)
	{
		pthread_mutex_lock(&phyl->arg->sync);
		phyl->arg->hungry--;
		pthread_mutex_unlock(&phyl->arg->sync);
	}
}

void	*phylo_life(void *arg)
{
	t_phyl *phyl;

	phyl = (t_phyl*)arg;
	phyl->eat = phyl->arg->s_tv;
	while (phyl->arg->state && (phyl->arg->hungry > 0))
	{
		take_forks(arg);
		pthread_mutex_lock(&phyl->dead);
		gettimeofday(&phyl->eat, 0);
		print_message("%ld %d is eating\n", arg);
		pthread_mutex_unlock(&phyl->conf);
		pthread_mutex_unlock(&phyl->dead);
		waitms(phyl->arg->t_eat, phyl->arg);
		pthread_mutex_unlock(phyl->lfork);
		pthread_mutex_unlock(phyl->rfork);
		sync_hungry(phyl);
		print_message("%ld %d is sleeping\n", arg);
		waitms(phyl->arg->t_sleep, phyl->arg);
		print_message("%ld %d is thinking\n", arg);
	}
	pthread_mutex_unlock(&phyl->conf);
	return (0);
}

int		parse(t_phyl *phyls, char const *argv[])
{
	phyls->arg->t_die = ft_atoi(argv[2]) * 1000;
	phyls->arg->t_eat = ft_atoi(argv[3]) * 1000;
	phyls->arg->t_sleep = ft_atoi(argv[4]) * 1000;
	phyls->arg->hungry = phyls->arg->number;
	if (argv[5])
		phyls->arg->dinners = ft_atoi(argv[5]);
	else
		phyls->arg->dinners = -1;
	phyls->arg->state = 1;
	return (0);
}

void	*check_death(void *arg)
{
	t_phyl *phyl;

	phyl = (t_phyl*)arg;
	pthread_mutex_lock(&(phyl->start));
	while (phyl->arg->state && (phyl->arg->hungry > 0))
	{
		pthread_mutex_lock(&phyl->dead);
		if (getms(phyl->eat) > phyl->arg->t_die && !(phyl->arg->state = 0))
		{
			pthread_mutex_lock(&phyl->arg->print);
			print_message_dead("%ld %d died\n", phyl);
			exit(0);
		}
		pthread_mutex_unlock(&phyl->dead);
		usleep(3000);
	}
	return (0);
}

int		init(t_phyl *phyls, pthread_mutex_t *forks)
{
	int	i;

	i = -1;
	while (++i < phyls->arg->number)
	{
		pthread_mutex_init(&(forks[i]), 0);
		pthread_mutex_init(&(phyls[i].permf), 0);
		pthread_mutex_init(&(phyls[i].conf), 0);
		pthread_mutex_init(&(phyls[i].start), 0);
		pthread_mutex_init(&(phyls[i].dead), 0);
		pthread_mutex_lock(&(phyls[i].permf));
		pthread_mutex_lock(&(phyls[i].conf));
		pthread_mutex_lock(&(phyls[i].start));
		phyls[i].dinners = phyls->arg->dinners;
		phyls[i].arg = phyls->arg;
		phyls[i].number = i;
		phyls[i].lfork = &forks[i];
		if (i == phyls->arg->number - 1)
			phyls[i].rfork = &forks[0];
		else
			phyls[i].rfork = &forks[i + 1];
		pthread_create(&phyls[i].phylo, NULL, phylo_life, &phyls[i]);
		pthread_create(&phyls[i].mon, NULL, check_death, &phyls[i]);
	}
	return (0);
}
