/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 12:03:33 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/16 14:57:30 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct info
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	bool			philo_dead;
	int				num_of_philo;
	pthread_mutex_t	dlock;
	pthread_mutex_t	print_lock;
}	t_info;

typedef struct philo
{
	int				id;
	int				fork;
	int				m_fork;
	long			life;
	int				times_to_eat;
	pthread_t		thread;
	pthread_mutex_t	lock;
	pthread_mutex_t	rlock;
	struct timeval	start_t;
	struct timeval	life_t;
	struct info		*philo_info;
	struct philo	*next;
}	t_philo;

/* ----- PHILO ACTION ----- */
int		*handle_single_philo(t_philo *philo, int ttime);
bool	philo_eat(t_philo *philo);
int		is_philo_dead(t_philo *philo);

/* ----- MUTEX LOCK/UNLOCK AND TIME CALCULATION FUNCTIONS ----- */
long	n_timestamp(struct timeval *time);
size_t	timestamp_new(void);
void	ft_mutex(t_philo *philo, int lock_flag);
void	lock_order(t_philo *philo);

/* ----- INITIALIZE PHILO DATA / PRINT DATA / PARSING ----- */
void	print_info(t_philo *philo, int print_order);
bool	init_philo(t_philo *philo, int argc, char **argv);
bool	ft_error(int argc, char **argv);

/* ----- UTILS FUNCTIONS ----- */
void	ft_putchar(char c);
void	ft_putstr(char *s);
size_t	ft_strlen(const char *s);
void	ft_putnbr(int n);
int		ft_atoi(const char	*str);

#endif