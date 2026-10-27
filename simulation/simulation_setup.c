/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:42:27 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 17:01:12 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_sim_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->print_mutex);
		return (0);
	}
	return (1);
}

static void	init_simulation(t_sim *sim, t_config *config)
{
	sim->config = *config;
	sim->coders = NULL;
	sim->dongles = NULL;
	sim->start_time = 0;
	sim->stopped = 0;
}

int	start_simulation(t_sim *sim, t_config *config)
{
	init_simulation(sim, config);
	if (!allocate_arrays(sim))
		return (0);
	if (!init_dongles(sim))
	{
		free_arrays(sim);
		return (0);
	}
	if (!init_coders(sim))
	{
		destroy_dongles(sim, sim->config.numbers_of_coders);
		free_arrays(sim);
		return (0);
	}
	link_coders_to_dongles(sim);
	if (!init_sim_mutexes(sim))
	{
		destroy_coder_mutexes(sim);
		destroy_dongles(sim, sim->config.numbers_of_coders);
		free_arrays(sim);
		return (0);
	}
	return (1);
}

int	handle_join_failure(t_sim *sim)
{
	stop_simulation(sim);
	join_monitor_thread(sim);
	cleanup_sim(sim);
	return (1);
}

int	finish_simulation(t_sim	*sim)
{
	stop_simulation(sim);
	if (!join_monitor_thread(sim))
	{
		cleanup_sim(sim);
		return (1);
	}
	cleanup_sim(sim);
	return (0);
}
