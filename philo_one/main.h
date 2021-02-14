/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgertrud <msnazarow@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 01:02:27 by sgertrud          #+#    #+#             */
/*   Updated: 2021/02/14 18:49:17 by sgertrud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# include <sys/time.h>
# include <pthread.h>

typedef struct	s_arg
{
	int				number;
	int				hungry;
	int				dinners;
	char			state;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	struct timeval	s_tv;
	pthread_mutex_t	print;
	pthread_mutex_t	sync;
	pthread_t		order;
}				t_arg;

typedef struct	s_phyl
{
	pthread_t		phylo;
	pthread_t		mon;
	int				number;
	int				dinners;
	pthread_mutex_t	permf;
	pthread_mutex_t	start;
	pthread_mutex_t	conf;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	pthread_mutex_t	dead;
	struct timeval	eat;
	t_arg			*arg;
}				t_phyl;
int				min(int a, int b);
int				ft_atoi(const char *str);
time_t			getms(struct timeval start);
int				print_message(char *msg, t_phyl *phyl);
int				print_message_dead(char *msg, t_phyl *phyl);
void			*take_forks(t_phyl *phyl);
void			*waitms(int ms, t_arg *arg);
int				init(t_phyl *phyls, pthread_mutex_t *forks);
int				parse(t_phyl *phyls, char const *argv[]);
int				max(int a, int b);

#endif
