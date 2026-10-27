/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:43:20 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 16:08:58 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_usage(void)
{
	const char	*message;

	message = "Usage: ./codexion number_of_coders time_to_burnout "
		"time_to_compile time_to_debug time_to_refactor "
		"number_of_compiles_required dongle_cooldown fifo|edf\n";
	write(2, message, strlen(message));
}

int	parse_and_validate(int argc, char **argv, t_config *config)
{
	if (!parse_arguments(argc, argv, config))
	{
		print_usage();
		return (0);
	}
	return (1);
}

int	prepare_simulation(t_sim *sim, t_config *config)
{
	if (!start_simulation(sim, config))
		return (0);
	if (!initialize_start_time(sim) || !check_initial_time(sim))
	{
		cleanup_sim(sim);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_sim		sim;

	if (!parse_and_validate(argc, argv, &config))
		return (1);
	if (!prepare_simulation(&sim, &config))
		return (1);
	if (!start_threads(&sim))
		return (1);
	if (!join_coder_threads(&sim, config.numbers_of_coders))
		return (handle_join_failure(&sim));
	return (finish_simulation(&sim));
}
