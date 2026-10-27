/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_queue.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:40:42 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/27 18:59:30 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_has_priority(t_request *a, t_request *b, t_policy policy)
{
	if (policy == FIFO)
	{
		if (a->order != b->order)
			return (a->order < b->order);
		return (a->coder_id < b->coder_id);
	}
	if (policy == EDF)
	{
		if (a->deadline != b->deadline)
			return (a->deadline < b->deadline);
		if (a->order != b->order)
			return (a->order < b->order);
		return (a->coder_id < b->coder_id);
	}
	return (0);
}

int	heap_insert(t_heap *heap, t_request request, t_policy policy)
{
	t_request	tmp;

	if (heap->size >= 2)
		return (0);
	heap->data[heap->size++] = request;
	if (heap->size == 2
		&& request_has_priority(&heap->data[1], &heap->data[0], policy))
	{
		tmp = heap->data[0];
		heap->data[0] = heap->data[1];
		heap->data[1] = tmp;
	}
	return (1);
}

int	heap_top_is_coder(t_heap *heap, int coder_id)
{
	if (heap->size == 0)
		return (0);
	return (heap->data[0].coder_id == coder_id);
}

int	heap_remove_top(t_heap *heap, t_request *out)
{
	if (heap->size == 0)
		return (0);
	if (out)
		*out = heap->data[0];
	if (heap->size == 2)
		heap->data[0] = heap->data[1];
	heap->size--;
	return (1);
}

int	heap_remove_coder(t_heap *heap, int coder_id)
{
	if (heap->size == 0)
		return (0);
	if (heap->data[0].coder_id == coder_id)
		return (heap_remove_top(heap, NULL));
	if (heap->size == 2 && heap->data[1].coder_id == coder_id)
	{
		heap->size = 1;
		return (1);
	}
	return (0);
}
