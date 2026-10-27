/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_request.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:41:02 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 16:32:43 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_request	build_request(t_coder *coder, t_dongle *dongle)
{
	t_request	request;

	request.coder_id = coder->id;
	request.order = dongle->next_order++;
	request.deadline = get_last_compile_start(coder)
		+ coder->sim->config.time_to_burnout;
	return (request);
}

static int	dongle_is_available(t_dongle *dongle)
{
	return (!dongle->in_use && get_time_ms() >= dongle->available_at);
}

static int	dongle_grant_access(t_coder *coder, t_dongle *dongle)
{
	if (!heap_top_is_coder(&dongle->waiting, coder->id))
		return (0);
	if (!dongle_is_available(dongle))
		return (0);
	dongle->in_use = 1;
	heap_remove_top(&dongle->waiting, NULL);
	return (1);
}

static int	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	timeout;

	while (!simulation_stopped(coder->sim))
	{
		if (dongle_grant_access(coder, dongle))
			return (1);
		if (!dongle->in_use && get_time_ms() < dongle->available_at)
		{
			timeout.tv_sec = dongle->available_at / 1000;
			timeout.tv_nsec = (dongle->available_at % 1000) * 1000000L;
			pthread_cond_timedwait(&dongle->cond,
				&dongle->mutex, &timeout);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	heap_remove_coder(&dongle->waiting, coder->id);
	return (0);
}

int	dongle_request_access(t_coder *coder, t_dongle *dongle)
{
	t_request	request;
	int			granted;

	pthread_mutex_lock(&dongle->mutex);
	request = build_request(coder, dongle);
	if (!heap_insert(&dongle->waiting, request, coder->sim->config.policy))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	granted = wait_for_dongle(coder, dongle);
	pthread_mutex_unlock(&dongle->mutex);
	return (granted);
}
