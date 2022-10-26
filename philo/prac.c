/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prac.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 14:28:07 by mkhan             #+#    #+#             */
/*   Updated: 2022/10/26 21:26:13 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


long	n_timestamp(struct timeval *time)
{
	struct timeval c_time;
	long	res;
	
	gettimeofday(&c_time, NULL);
	
	res = ((c_time.tv_sec - time->tv_sec) * 1000) + ((c_time.tv_usec - time->tv_usec) / 1000);
	return (res);
	// return (((c_time.tv_sec - time->tv_sec) * 1000) + ((c_time.tv_usec - time->tv_usec) / 1000));
}

size_t	timestamp_new(void)
{
	struct timeval c_time;
	size_t	time;
	
	gettimeofday(&c_time, NULL);
	time = (c_time.tv_sec * 1000000 + c_time.tv_usec);
	return (time);
	// return ((c_time.tv_sec * 1000000) + (c_time.tv_usec));
}

void	init_info(t_philo	*philo, int argc, char **argv)
{
	philo->fork = 0;
	philo->m_fork = 0;
	philo->time_to_die = ft_atoi(argv[2]);
	philo->time_to_eat = ft_atoi(argv[3]);
	philo->time_to_sleep = ft_atoi(argv[4]);
	pthread_mutex_init(&philo->lock, NULL);
	pthread_mutex_init(&philo->rlock, NULL);
	philo->key->no_phil = ft_atoi(argv[1]);
	if (argc == 6)
		philo->rounds = ft_atoi(argv[5]);
	else
		philo->rounds = -1;
}

bool	init_philo(t_philo	*philo, int argc, char **argv)
{
	int	i;
	t_info	*philo_info;

	i = 0;
	philo_info = malloc(sizeof(t_info));
	if (!philo_info)
		return (false);
	philo_info->p_dead = 0;
	pthread_mutex_init(&philo_info->print_lock, NULL);
	pthread_mutex_init(&philo_info->dlock, NULL);
	while (i < ft_atoi(argv[1]))
	{
		philo[i].key = philo_info;
		philo[i].id = i + 1;
		init_info(&philo[i], argc, argv);
		gettimeofday(&philo[i].life_t, NULL);
		philo[i].life = n_timestamp(&philo[i].life_t);
		if (i == (ft_atoi(argv[1]) - 1))
			philo[i].next = &philo[0];
		else
			philo[i].next = &philo[i + 1];
		i++;
	}
	return (true);
}

int	*single_t_sleep(t_philo *philo, int ttime)
{
	size_t	ts;
	size_t	target;
	long	n_time;

	ts = timestamp_new();
	target = ts + (ttime * 1000);
	while (ts < target)
	{
		n_time = n_timestamp(&philo->life_t);
		if ((n_time - philo->life) > (philo->time_to_die))
			break ;
		ts = timestamp_new();
	}
	ft_putnbr(n_timestamp(&philo->life_t));
	ft_putchar(' ');
	ft_putnbr(philo->id);
	ft_putstr(" died\n");
	return (NULL);
}

void	ft_muteall(t_philo *philo, int lock_flag)
{
	if (lock_flag)
		pthread_mutex_lock(&philo->key->dlock);
	else
		pthread_mutex_unlock(&philo->key->dlock);
}

void	print_info(t_philo	*philo, int print_order)
{
	ft_muteall(philo, 1);
	if (!philo->key->p_dead)
	{
		pthread_mutex_lock(&philo->key->print_lock);
		ft_putnbr(n_timestamp(&philo->life_t));
		ft_putchar(' ');
		ft_putnbr(philo->id);
		if (print_order == 1)
			ft_putstr(" has taken a fork\n");
		else if (print_order == 2)
			ft_putstr(" is eating\n");
		else if (print_order == 3)
			ft_putstr(" is sleeping\n");
		else if (print_order == 4)
			ft_putstr(" is thinking\n");
		else if (print_order == 5)
		{
			ft_putstr(" died\n");
			philo->key->p_dead = 1;
		}
		pthread_mutex_unlock(&philo->key->print_lock);
	}
	ft_muteall(philo, 0);
}

int	is_dead(t_philo *philo)
{
	long	n_time;
	int		flag;
	
	flag = 0;
	ft_muteall(philo, 1);
	if (philo->key->p_dead)
		flag = 1;
	else
	{
		n_time = n_timestamp(&philo->life_t);
		if ((n_time - philo->life) > (philo->time_to_die))
		{
			ft_muteall(philo, 0);
			print_info(philo, 5);
			ft_muteall(philo, 1);
			philo->key->p_dead = 1;
			flag = 1;
		}
	}
	ft_muteall(philo, 0);
	return (flag);
}

void	lock_order(t_philo *philo)
{
	if (philo->id % philo->key->no_phil)
	{
		pthread_mutex_lock(&philo->lock);
		pthread_mutex_lock(&philo->next->lock);
	}
	else
	{
		pthread_mutex_lock(&philo->next->lock);
		pthread_mutex_lock(&philo->lock);
	}
}

int	fork_mask(t_philo *philo)
{
	if (philo->m_fork == philo->id || philo->next->m_fork == philo->id)
		return (0);
	return (1);
}

int	t_sleep(t_philo *philo, int khana_time)
{
size_t	ts;
size_t	target;

ts = timestamp_new();
target = ts + (khana_time * 1000);
while (ts < target)
{
	if (is_dead(philo))
		return (1);
	usleep(100);
	ts = timestamp_new();
}
return (0);
}

bool	ft_sleep(t_philo *philo)
{
	size_t	ts;
	size_t	target;

	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(&philo->next->lock);
	ts = timestamp_new();
	target = ts + (philo->time_to_sleep * 1000);
	while (ts < target)
	{
		if (is_dead(philo))
			return (false);
		usleep(100);
		ts = timestamp_new();
	}
	if (philo->id % philo->key->no_phil)
	{
		pthread_mutex_lock(&philo->lock);
		pthread_mutex_lock(&philo->next->lock);
	}
	else
	{
		pthread_mutex_lock(&philo->next->lock);
		pthread_mutex_lock(&philo->lock);
	}
	return (true);
}

bool	eat(t_philo *philo)
{
	philo->fork = 1;
	philo->next->fork = 1;
	philo->m_fork = philo->id;
	philo->next->m_fork = philo->id;
	philo->life = n_timestamp(&philo->life_t);
	pthread_mutex_unlock(&philo->rlock);
	pthread_mutex_unlock(&philo->lock);
	pthread_mutex_unlock(&philo->next->lock);
	print_info(philo, 1);
	print_info(philo, 1);
	print_info(philo, 2);
	philo->rounds--;
	if (t_sleep(philo, philo->time_to_eat))
		return (false);
	lock_order(philo);
	philo->fork = 0;
	philo->next->fork = 0;
	print_info(philo, 3);
	if (!ft_sleep(philo))
		return (false);
	print_info(philo, 4);
	pthread_mutex_lock(&philo->rlock);
	return (true);
}

void	*routine(void *philo_data)
{
	t_philo	*philo;
	
	philo = (t_philo *) philo_data;
	// while (philo->key->no_phil == 1)
	// 	return (single_t_sleep(philo, philo->time_to_die)); //change the parameters for the function
	while (1)
	{
		if (is_dead(philo) || !philo->rounds)
			break ;
		lock_order(philo);
		pthread_mutex_lock(&philo->rlock);
		if (!(philo->fork) && !(philo->next->fork) && philo->rounds && fork_mask(philo))
		{
			if (!eat(philo))
				return (NULL);
		}
		pthread_mutex_unlock(&philo->rlock);
		pthread_mutex_unlock(&philo->lock);
		pthread_mutex_unlock(&philo->next->lock);
		usleep(100);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;
	int		i;
	
	// if (ft_error(argc, argv))
	// 	return (1);
	philo = malloc(sizeof(t_philo) * ft_atoi(argv[1]));
	if (!philo)
		return (1);
	// ph_create_threads(); --- create threads
	if (!init_philo(philo, argc, argv))
		return (1);
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		pthread_create(&philo[i].thread, NULL, routine, (void *) &philo[i]);
		usleep(100);
		i++;
	}
	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	free(philo);
	return (0);
}