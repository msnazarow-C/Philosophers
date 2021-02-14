/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phylo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 18:41:22 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 19:26:50 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	*phylo_life(void *arg)
{
	t_phyl *phyl;

	phyl = (t_phyl*)arg;
	phyl->eat = phyl->arg->s_tv;
	while (phyl->arg->state && (phyl->arg->hungry > 0))
	{
		sem_wait(phyl->permf);
		take_forks(arg);
		sem_wait(phyl->dead);
		gettimeofday(&phyl->eat, 0);
		print_message("%ld %d is eating\n", arg);
		sem_post(phyl->conf);
		sem_post(phyl->dead);
		waitms(phyl->arg->t_eat, phyl->arg);
		sem_post(phyl->arg->forks);
		sem_post(phyl->arg->forks);
		phyl->dinners--;
		if (!phyl->dinners)
			sem_post(phyl->sync);
		print_message("%ld %d is sleeping\n", arg);
		waitms(phyl->arg->t_sleep, phyl->arg);
		print_message("%ld %d is thinking\n", arg);
	}
	sem_post(phyl->conf);
	return (0);
}
