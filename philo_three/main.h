/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 01:02:27 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 19:21:24 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <string.h>

typedef struct	s_arg
{
	int				number;
	int				dinners;
	int				hungry;
	char			state;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	struct timeval	s_tv;
	sem_t			*kill_;
	sem_t			*print;
	sem_t			*forks;
	pthread_t		order;
}				t_arg;

typedef struct	s_phyl
{
	pthread_t		phylo;
	pthread_t		mon;
	int				number;
	int				dinners;
	sem_t			*permf;
	sem_t			*conf;
	sem_t			*dead;
	sem_t			*start;
	sem_t			*end;
	sem_t			*sync;
	struct timeval	eat;
	t_arg			*arg;
}				t_phyl;

int				ft_strlen(const char *s);
int				min(int a, int b);
int				ft_atoi(const char *str);
int				getms(struct timeval start);
int				print_message(char *msg, t_phyl *phyl);
int				print_message_dead(char *msg, t_phyl *phyl);
void			*take_forks(t_phyl *phyl);
void			*waitms(int ms, t_arg *arg);
int				init(t_phyl *phyls, pid_t *pids);
int				parse(t_phyl *phyls, char const *argv[]);
int				max(int a, int b);
void			ft_itoa(int n, char s[]);
void			*phylo_life(void *arg);
#endif
