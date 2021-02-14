/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 00:13:18 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 19:24:08 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

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
	sem_wait(phyl->start);
	while (phyl->arg->state && (phyl->arg->hungry > 0))
	{
		sem_wait(phyl->dead);
		if (getms(phyl->eat) > phyl->arg->t_die && !(phyl->arg->state = 0))
		{
			sem_wait(phyl->arg->print);
			print_message_dead("%ld %d died\n", phyl);
			exit(0);
		}
		sem_post(phyl->dead);
		usleep(3000);
	}
	return (0);
}

sem_t	*make_sem(char p, int i)
{
	char	name[10];
	sem_t	*sem;

	name[0] = p;
	ft_itoa(i, &name[1]);
	sem = sem_open(name, O_CREAT | O_EXCL, 0644, p == 'd');
	sem_unlink(name);
	return (sem);
}

int		init(t_phyl *phyls)
{
	int	i;

	i = -1;
	while (++i < phyls->arg->number)
	{
		phyls[i].permf = make_sem('p', i);
		phyls[i].conf = make_sem('c', i);
		phyls[i].dead = make_sem('d', i);
		phyls[i].start = make_sem('s', i);
		phyls[i].dinners = phyls->arg->dinners;
		phyls[i].arg = phyls->arg;
		phyls[i].number = i;
		pthread_create(&phyls[i].phylo, NULL, phylo_life, &phyls[i]);
		pthread_create(&phyls[i].mon, NULL, check_death, &phyls[i]);
	}
	return (0);
}
