/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:41:19 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 15:33:27 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongle(t_dongle *dongle, int id)
{
	dongle->id = id;
	dongle->in_use = 0;
	dongle->available_at = 0;
	dongle->next_order = 0;
	dongle->waiting.size = 0;
	if (!heap_init(&dongle->waiting))
		return (0);
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&dongle->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	return (1);
}

int	init_dongles(t_sim	*sim)
{
	int	i;
	int	count;

	count = sim->config.numbers_of_coders;
	i = 0;
	while (i < count)
	{
		if (!init_dongle(&sim->dongles[i], i + 1))
		{
			destroy_dongles(sim, count);
			return (0);
		}
		i++;
	}
	return (1);
}

void	dongle_release(t_coder	*coder, t_dongle	*dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = 0;
	dongle->available_at = get_time_ms()
		+ coder->sim->config.dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	destroy_dongles(t_sim	*sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		heap_destroy(&sim->dongles[i].waiting);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
}
