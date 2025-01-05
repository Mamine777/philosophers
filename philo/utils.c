/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:53:11 by mokariou          #+#    #+#             */
/*   Updated: 2024/12/30 15:38:44 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	res;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (10 * res) + (str[i++] - 48);
		if (res > 9223372036854775807 && sign > 0)
			return (-1);
		if (res > 9223372036854775807 && sign < 0)
			return (0);
	}
	return ((int)(res * sign));
}

long long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	log_action(t_philo *philo, int id, char *str)
{
	pthread_mutex_lock(&philo->table->print);
	pthread_mutex_lock(&philo->table->check_death);
	if (!philo->table->stop_simulation)
	{
		printf("%lld %d %s\n", get_time() - philo->table->first_timestamp, id,
			str);
	}
	pthread_mutex_unlock(&philo->table->check_death);
	pthread_mutex_unlock(&philo->table->print);
	return ;
}

void	u_got_knocked_out(long long time, t_philo *philo)
{
	long long	i;

	i = get_time();
	while (1)
	{
		pthread_mutex_lock(&philo->table->check_death);
		if (philo->table->stop_simulation)
		{
			pthread_mutex_unlock(&philo->table->check_death);
			break ;
		}
		pthread_mutex_unlock(&philo->table->check_death);
		if ((get_time() - i) >= time)
			break ;
		usleep(50);
	}
}

void	rest_actions(t_philo *philo)
{
	log_action(philo, philo->id, "is sleeping");
	u_got_knocked_out(philo->schedule->sleep_time, philo);
	log_action(philo, philo->id, "is thinking");
}
