/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:44:50 by mokariou          #+#    #+#             */
/*   Updated: 2024/12/30 15:04:40 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&(philo->table->forks[philo->left_fork]));
	log_action(philo, philo->id, "has taken a fork");
	pthread_mutex_lock(&(philo->table->forks[philo->right_fork]));
	log_action(philo, philo->id, "has taken a fork");
	pthread_mutex_lock(&(philo->table->meal_check));
	log_action(philo, philo->id, "is eating");
	philo->last_meal = get_time();
	philo->x_meals++;
	pthread_mutex_unlock(&(philo->table->meal_check));
	u_got_knocked_out(philo->schedule->eat_time, philo);
	pthread_mutex_unlock(&(philo->table->forks[philo->left_fork]));
	pthread_mutex_unlock(&(philo->table->forks[philo->right_fork]));
}

int	init_mutex(t_table *table)
{
	int	i;

	i = -1;
	while (++i, i < table->num_of_philos)
	{
		if (pthread_mutex_init(&table->forks[i], NULL))
			return (1);
	}
	if (pthread_mutex_init(&(table->print), NULL))
		return (1);
	if (pthread_mutex_init(&(table->meal_check), NULL))
		return (1);
	if (pthread_mutex_init(&(table->check_death), NULL))
		return (1);
	if (pthread_mutex_init(&(table->check_ate), NULL))
		return (1);
	return (0);
}

int	init_philo(t_table *table)
{
	int	i;

	if (init_mutex(table))
		return (1);
	i = -1;
	while (++i, i < table->num_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].x_meals = 0;
		table->philos[i].left_fork = i;
		table->philos[i].right_fork = (i + 1) % table->num_of_philos;
		table->philos[i].last_meal = 0;
		table->philos[i].schedule = &table->schedule;
		table->philos[i].table = table;
	}
	return (0);
}

int	fill_table(t_table *table, char **av)
{
	if (!av[1] || !av[2] || !av[3] || !av[4])
		return (1);
	table->num_of_philos = ft_atoi(av[1]);
	table->schedule.time_to_die = ft_atoi(av[2]);
	table->schedule.eat_time = ft_atoi(av[3]);
	table->schedule.sleep_time = ft_atoi(av[4]);
	table->all_ate = 0;
	table->stop_simulation = 0;
	if (table->num_of_philos < 1 || table->schedule.time_to_die < 0
		|| table->schedule.eat_time < 0 || table->schedule.sleep_time < 0
		|| table->num_of_philos > 200)
		return (1);
	if (table->num_of_philos == 1)
		return (printf("0 1 has taken a fork\n"), printf("0 1 has died\n"), 1);
	if (av[5])
	{
		table->schedule.num_of_to_eat = ft_atoi(av[5]);
		if (table->schedule.num_of_to_eat <= 0)
			return (1);
	}
	else
		table->schedule.num_of_to_eat = -1;
	if (init_philo(table))
		return (1);
	return (0);
}
