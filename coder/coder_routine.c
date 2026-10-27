/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:09:13 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/27 18:26:05 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ordered_dongles(t_coder *coder,
				t_dongle **first, t_dongle **second)
{
	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

static int	acquire_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	ordered_dongles(coder, &first, &second);
	if (!dongle_request_access(coder, first))
		return (0);
	print_status(coder, "has taken a dongle");
	if (!dongle_request_access(coder, second))
	{
		dongle_release(coder, first);
		return (0);
	}
	return (1);
}

static void	release_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	ordered_dongles(coder, &first, &second);
	dongle_release(coder, first);
	dongle_release(coder, second);
}

static void	compile_with_dongles(t_coder *coder)
{
	print_compile_start(coder);
	smart_sleep(coder->sim, coder->sim->config.time_to_compile);
	increment_compile_count(coder);
	if (all_coders_finished(coder->sim))
		stop_simulation(coder->sim);
	release_dongles(coder);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 != 0)
		usleep(1000);
	while (!simulation_stopped(coder->sim))
	{
		if (!acquire_dongles(coder))
			continue ;
		compile_with_dongles(coder);
		print_status(coder, "is debugging");
		smart_sleep(coder->sim, coder->sim->config.time_to_debug);
		if (simulation_stopped(coder->sim))
			break ;
		print_status(coder, "is refactoring");
		smart_sleep(coder->sim, coder->sim->config.time_to_refactor);
	}
	return (NULL);
}
