/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 14:29:19 by mokariou          #+#    #+#             */
/*   Updated: 2024/12/30 15:43:15 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	threads2(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&(table->meal_check));
	if (table->schedule.num_of_to_eat != -1
		&& philo->x_meals >= table->schedule.num_of_to_eat)
	{
		pthread_mutex_unlock(&(table->meal_check));
		return (2);
	}
	pthread_mutex_unlock(&(table->meal_check));
	take_forks(philo);
	pthread_mutex_lock(&table->check_ate);
	if (table->all_ate)
	{
		pthread_mutex_unlock(&table->check_ate);
		return (2);
	}
	pthread_mutex_unlock(&table->check_ate);
	return (0);
}

void	*threads(void *arg)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)arg;
	table = philo->table;
	if (philo->id % 2 == 0)
		usleep(15000);
	while (1)
	{
		pthread_mutex_lock(&table->check_death);
		if (table->stop_simulation)
		{
			pthread_mutex_unlock(&table->check_death);
			break ;
		}
		pthread_mutex_unlock(&table->check_death);
		if (threads2(table, philo) == 2)
			break ;
		rest_actions(philo);
	}
	return (NULL);
}

static int	check_philos_death2(t_philo *philo, t_table *table, int i)
{
	pthread_mutex_lock(&(table->meal_check));
	if (get_time() - philo[(i)].last_meal >= table->schedule.time_to_die)
	{
		log_action(philo, (i + 1), "has died");
		pthread_mutex_lock(&table->check_death);
		table->stop_simulation = 1;
		pthread_mutex_unlock(&table->check_death);
		pthread_mutex_unlock(&(table->meal_check));
		return (2);
	}
	pthread_mutex_unlock(&(table->meal_check));
	return (0);
}

void	check_philos_death(t_table *table)
{
	t_philo	*philo;
	int		i;

	philo = table->philos;
	i = -1;
	while (++i < table->num_of_philos)
	{
		pthread_mutex_lock(&table->check_death);
		if (table->stop_simulation)
		{
			pthread_mutex_unlock(&table->check_death);
			break ;
		}
		pthread_mutex_unlock(&table->check_death);
		if (check_philos_death2(philo, table, i) == 2)
			break ;
		usleep(100);
	}
}

int	routine(t_table *table)
{
	t_philo	*philo;
	int		i;

	table->first_timestamp = get_time();
	philo = table->philos;
	i = -1;
	while (++i, i < table->num_of_philos)
	{
		if (pthread_create(&(philo[i].thread_id), NULL, threads, &(philo[i])))
		{
			pthread_mutex_lock(&table->check_death);
			table->stop_simulation = 1;
			pthread_mutex_unlock(&table->check_death);
			while (--i >= 0)
				pthread_join(philo[i].thread_id, NULL);
			return (1);
		}
		pthread_mutex_lock(&table->meal_check);
		philo[i].last_meal = get_time();
		pthread_mutex_unlock(&table->meal_check);
	}
	guard_death(table);
	destroy(table);
	return (0);
}
