/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:43:38 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 16:39:47 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_non_negative_long(const char *str, long *result)
{
	long	value;
	int		digit;
	int		i;

	if (str == NULL || str[0] == '\0')
		return (0);
	value = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		digit = str[i] - '0';
		if (value > (LONG_MAX - digit) / 10)
			return (0);
		value = value * 10 + digit;
		i++;
	}
	*result = value;
	return (1);
}

static int	parse_numeric_arguments(char **argv, long values[7])
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (!parse_non_negative_long(argv[i + 1], &values[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	parse_policy(const char *str, t_policy *policy)
{
	if (strcmp(str, "fifo") == 0)
		*policy = FIFO;
	else if (strcmp(str, "edf") == 0)
		*policy = EDF;
	else
		return (0);
	return (1);
}

static void	assign_config(t_config *config, long values[7])
{
	config->numbers_of_coders = (int)values[0];
	config->time_to_burnout = values[1];
	config->time_to_compile = values[2];
	config->time_to_debug = values[3];
	config->time_to_refactor = values[4];
	config->numbers_of_compile_required = values[5];
	config->dongle_cooldown = values[6];
}

int	parse_arguments(int argc, char **argv, t_config *config)
{
	long	values[7];

	if (argc != 9)
		return (0);
	if (!parse_numeric_arguments(argv, values))
		return (0);
	if (values[0] == 0 || values[0] > INT_MAX)
		return (0);
	if (!parse_policy(argv[8], &config->policy))
		return (0);
	assign_config(config, values);
	return (1);
}
