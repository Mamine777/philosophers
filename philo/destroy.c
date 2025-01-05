/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 15:09:34 by mokariou          #+#    #+#             */
/*   Updated: 2024/12/30 15:40:51 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy(t_table *table)
{
	int	i;

	i = -1;
	while (++i, i < table->num_of_philos)
	{
		pthread_join(table->philos[i].thread_id, NULL);
	}
	i = -1;
	while (++i < table->num_of_philos)
		pthread_mutex_destroy(&(table->forks[i]));
	pthread_mutex_destroy(&(table->print));
	pthread_mutex_destroy(&(table->meal_check));
	pthread_mutex_destroy(&(table->check_death));
	pthread_mutex_destroy(&(table->check_ate));
}

int	checking(t_table *table)
{
	check_philos_death(table);
	pthread_mutex_lock(&table->check_death);
	if (table->stop_simulation)
	{
		pthread_mutex_unlock(&table->check_death);
		return (2);
	}
	pthread_mutex_unlock(&table->check_death);
	return (0);
}

void	guard_death(t_table *table)
{
	int	i;

	while (1)
	{
		pthread_mutex_lock(&table->check_ate);
		if (table->all_ate)
		{
			break ;
			pthread_mutex_unlock(&table->check_ate);
		}
		pthread_mutex_unlock(&table->check_ate);
		if (checking(table) == 2)
			break ;
		i = 0;
		while (table->schedule.num_of_to_eat != -1 && i < table->num_of_philos
			&& table->philos[i].x_meals >= table->schedule.num_of_to_eat)
			i++;
		pthread_mutex_lock(&table->check_ate);
		if (i == table->num_of_philos)
			table->all_ate = 1;
		pthread_mutex_unlock(&table->check_ate);
	}
}
