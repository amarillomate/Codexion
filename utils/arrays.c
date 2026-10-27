/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrays.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:43:55 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 16:34:20 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	allocate_arrays(t_sim *sim)
{
	int	count;

	count = sim->config.numbers_of_coders;
	sim->coders = malloc(sizeof(*sim->coders) * count);
	if (sim->coders == NULL)
		return (0);
	sim->dongles = malloc(sizeof(*sim->dongles) * count);
	if (sim->dongles == NULL)
	{
		free_arrays(sim);
		return (0);
	}
	return (1);
}

void	free_arrays(t_sim *sim)
{
	free(sim->dongles);
	free(sim->coders);
	sim->dongles = NULL;
	sim->coders = NULL;
}
