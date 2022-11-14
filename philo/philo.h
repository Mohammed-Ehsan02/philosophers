/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 12:03:33 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/10 13:16:17 by mkhan            ###   ########.fr       */
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

/* ----- Utils functions ----- */
void	ft_putchar(char c);
void	ft_putstr(char *s);
size_t	ft_strlen(const char *s);
void	ft_putnbr(int n);
int		ft_atoi(const char	*str);

#endif