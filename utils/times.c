/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   times.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:45:16 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/27 18:44:31 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return (time.tv_sec * 1000L + time.tv_usec / 1000L);
}

void	smart_sleep(t_sim *sim, long time_ms)
{
	long	start;

	start = get_time_ms();
	while (!simulation_stopped(sim))
	{
		if ((get_time_ms() - start) >= time_ms)
			break ;
		usleep(1000);
	}
}

int	initialize_start_time(t_sim *sim)
{
	int		i;
	long	start;

	start = get_time_ms();
	if (start == -1)
		return (0);
	sim->start_time = start;
	i = 0;
	while (i < sim->config.numbers_of_coders)
	{
		set_last_compile_start(&sim->coders[i], start);
		i++;
	}
	return (1);
}

int	check_initial_time(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.numbers_of_coders)
	{
		if (get_compile_count(&sim->coders[i]) != 0)
			return (0);
		if (get_last_compile_start(&sim->coders[i])
			!= sim->start_time)
			return (0);
		i++;
	}
	return (1);
}
