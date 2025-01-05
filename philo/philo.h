/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokariou <mokariou@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:21:59 by mokariou          #+#    #+#             */
/*   Updated: 2024/12/30 15:43:23 by mokariou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct t_schedule
{
	int				time_to_die;
	int				sleep_time;
	int				eat_time;
	int				num_of_to_eat;
}					t_schedule;

typedef struct t_philo
{
	int				id;
	int				x_meals;
	int				left_fork;
	int				right_fork;
	long			last_meal;
	pthread_t		thread_id;
	t_schedule		*schedule;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	int				num_of_philos;
	int				stop_simulation;
	int				all_ate;
	long long		first_timestamp;
	pthread_mutex_t	meal_check;
	pthread_mutex_t	forks[200];
	pthread_mutex_t	print;
	pthread_mutex_t	check_death;
	pthread_mutex_t	check_ate;
	t_philo			philos[200];
	t_schedule		schedule;

}					t_table;

int					ft_atoi(const char *str);
int					fill_table(t_table *table, char **av);
long long			get_time(void);
void				log_action(t_philo *philo, int id, char *str);
int					routine(t_table *table);
void				u_got_knocked_out(long long time, t_philo *philo);
void				rest_actions(t_philo *philo);
void				take_forks(t_philo *philo);
void				destroy(t_table *table);
void				guard_death(t_table *table);
void				check_philos_death(t_table *table);

#endif