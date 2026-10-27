/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:41:36 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/26 15:27:31 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap	*heap)
{
	if (heap == NULL)
		return (0);
	heap->size = 0;
	return (1);
}

void	heap_destroy(t_heap	*heap)
{
	if (heap == NULL)
		return ;
	heap->size = 0;
}
