/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhan <mkhan@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 14:31:36 by mkhan             #+#    #+#             */
/*   Updated: 2022/11/16 14:48:51 by mkhan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_info(t_philo *philo, int print_order)
{
	ft_mutex(philo, 1);
	if (!philo->philo_info->philo_dead)
	{
		pthread_mutex_lock(&philo->philo_info->print_lock);
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
			philo->philo_info->philo_dead = true;
		}
		pthread_mutex_unlock(&philo->philo_info->print_lock);
	}
	ft_mutex(philo, 0);
}

static	void	init_info(t_philo *philo, int argc, char **argv)
{
	philo->fork = 0;
	philo->m_fork = 0;
	philo->philo_info->time_to_die = ft_atoi(argv[2]);
	philo->philo_info->time_to_eat = ft_atoi(argv[3]);
	philo->philo_info->time_to_sleep = ft_atoi(argv[4]);
	pthread_mutex_init(&philo->lock, NULL);
	pthread_mutex_init(&philo->rlock, NULL);
	philo->philo_info->num_of_philo = ft_atoi(argv[1]);
	if (argc == 6)
		philo->times_to_eat = ft_atoi(argv[5]);
	else
		philo->times_to_eat = -1;
}

bool	init_philo(t_philo *philo, int argc, char **argv)
{
	int		i;
	t_info	*philo_info;

	i = 0;
	philo_info = malloc(sizeof(t_info));
	if (!philo_info)
		return (false);
	philo_info->philo_dead = false;
	pthread_mutex_init(&philo_info->print_lock, NULL);
	pthread_mutex_init(&philo_info->dlock, NULL);
	while (i < ft_atoi(argv[1]))
	{
		philo[i].philo_info = philo_info;
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

bool	ft_error(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		ft_putstr("Error : Run philo using ./philo 2 800 200 200 \
					((7) -- optional)");
		return (false);
	}
	i = 1;
	while (argv[i])
	{
		if (*argv[i] == '\0' || ft_atoi(argv[i]) <= 0)
		{
			ft_putstr("Error : Run philo using ./philo 2 800 200 200 \
						((7) --- optional)");
			return (false);
		}
		i++;
	}
	return (true);
}
