/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:10:07 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 16:40:59 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&coder->sim->print_mutex);
	if (!simulation_stopped(coder->sim))
	{
		timestamp = get_time_ms() - coder->sim->start_time;
		printf("%ld %d %s\n", timestamp, coder->id, status);
	}
	pthread_mutex_unlock(&coder->sim->print_mutex);
}

void	set_last_compile_start(t_coder *coder, long time_ms)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = time_ms;
	pthread_mutex_unlock(&coder->state_mutex);
}

long	get_last_compile_start(t_coder *coder)
{
	long	time_ms;

	pthread_mutex_lock(&coder->state_mutex);
	time_ms = coder->last_compile_start;
	pthread_mutex_unlock(&coder->state_mutex);
	return (time_ms);
}

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
}

int	get_compile_count(t_coder *coder)
{
	int	count;

	pthread_mutex_lock(&coder->state_mutex);
	count = coder->compile_count;
	pthread_mutex_unlock(&coder->state_mutex);
	return (count);
}
