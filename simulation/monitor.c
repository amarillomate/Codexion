/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:42:03 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/27 18:09:40 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_sim *sim)
{
	long	now;
	long	last_compile;
	int		i;

	i = 0;
	while (i < sim->config.numbers_of_coders)
	{
		now = get_time_ms();
		last_compile = get_last_compile_start(&sim->coders[i]);
		if ((now - last_compile) >= sim->config.time_to_burnout)
		{
			print_burnout(&sim->coders[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	all_coders_finished(t_sim *sim)
{
	long	required;
	int		i;

	required = sim->config.numbers_of_compile_required;
	if (required <= 0)
		return (0);
	i = 0;
	while (i < sim->config.numbers_of_coders)
	{
		if (get_compile_count(&sim->coders[i]) < required)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!simulation_stopped(sim))
	{
		if (check_burnout(sim))
			break ;
		if (all_coders_finished(sim))
		{
			stop_simulation(sim);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
