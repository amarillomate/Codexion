/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:09:48 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/27 19:00:12 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_coders(t_sim *sim)
{
	int	i;
	int	count;

	count = sim->config.numbers_of_coders;
	i = 0;
	while (i < count)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].compile_count = 0;
		sim->coders[i].left_dongle = NULL;
		sim->coders[i].right_dongle = NULL;
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].state_mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&sim->coders[i].state_mutex);
			return (0);
		}
		i++;
	}
	return (1);
}

void	link_coders_to_dongles(t_sim *sim)
{
	int	i;
	int	count;

	count = sim->config.numbers_of_coders;
	i = 0;
	while (i < count)
	{
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % count];
		i++;
	}
}

void	print_burnout(t_coder *coder)
{
	long	timestamp;

	pthread_mutex_lock(&coder->sim->print_mutex);
	if (!simulation_stopped(coder->sim))
	{
		timestamp = get_time_ms() - coder->sim->start_time;
		printf("%ld %d burned out\n", timestamp, coder->id);
		pthread_mutex_lock(&coder->sim->stop_mutex);
		coder->sim->stopped = 1;
		pthread_mutex_unlock(&coder->sim->stop_mutex);
	}
	pthread_mutex_unlock(&coder->sim->print_mutex);
	wake_all_dongles(coder->sim);
}

void	print_compile_start(t_coder *coder)
{
	long	now;
	long	start;

	pthread_mutex_lock(&coder->sim->print_mutex);
	if (!simulation_stopped(coder->sim))
	{
		now = get_time_ms();
		start = coder->sim->start_time;
		set_last_compile_start(coder, now);
		printf("%ld %d has taken a dongle\n", now - start, coder->id);
		printf("%ld %d is compiling\n", now - start, coder->id);
	}
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
