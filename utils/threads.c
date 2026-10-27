/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:45:31 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 17:14:33 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_coder_threads(t_sim *sim)
{
	int	i;
	int	count;

	i = 0;
	count = sim->config.numbers_of_coders;
	while (i < count)
	{
		if (pthread_create(&sim->coders[i].thread,
				NULL, coder_routine, &sim->coders[i]) != 0)
		{
			stop_simulation(sim);
			join_coder_threads(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	join_coder_threads(t_sim *sim, int count)
{
	int	i;
	int	success;

	success = 1;
	i = 0;
	while (i < count)
	{
		if (pthread_join(sim->coders[i].thread, NULL) != 0)
			success = 0;
		i++;
	}
	return (success);
}

int	start_monitor_thread(t_sim *sim)
{
	if (pthread_create(&sim->monitor, NULL,
			monitor_routine, sim) != 0)
		return (0);
	return (1);
}

int	join_monitor_thread(t_sim *sim)
{
	if (pthread_join(sim->monitor, NULL) != 0)
		return (0);
	return (1);
}

int	start_threads(t_sim *sim)
{
	if (!start_coder_threads(sim))
		return (0);
	if (!start_monitor_thread(sim))
	{
		stop_simulation(sim);
		join_coder_threads(sim, sim->config.numbers_of_coders);
		return (0);
	}
	return (1);
}
