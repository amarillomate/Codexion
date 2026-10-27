/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrgarc <andrgarc@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 12:49:00 by andrgarc          #+#    #+#             */
/*   Updated: 2026/10/27 18:20:13 by andrgarc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum e_policy
{
	FIFO,
	EDF
}	t_policy;

typedef struct s_config
{
	int			numbers_of_coders;
	long		time_to_burnout;
	long		time_to_compile;
	long		time_to_debug;
	long		time_to_refactor;
	long		numbers_of_compile_required;
	long		dongle_cooldown;
	t_policy	policy;
}	t_config;

typedef struct s_sim	t_sim;

typedef struct s_request
{
	int		coder_id;
	long	order;
	long	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	data[2];
	int			size;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	int				in_use;
	long			available_at;
	long			next_order;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_heap			waiting;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	long			last_compile_start;
	int				compile_count;
	pthread_t		thread;
	pthread_mutex_t	state_mutex;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_sim			*sim;
}	t_coder;

struct s_sim
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	long			start_time;
	int				stopped;
	pthread_t		monitor;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
};

int		parse_arguments(int argc, char **argv, t_config *config);
int		parse_and_validate(int argc, char **argv, t_config *config);
void	print_usage(void);

int		allocate_arrays(t_sim *sim);
void	free_arrays(t_sim *sim);

int		init_dongles(t_sim *sim);
void	dongle_release(t_coder *coder, t_dongle *dongle);
void	destroy_dongles(t_sim *sim, int count);

int		init_coders(t_sim *sim);
void	link_coders_to_dongles(t_sim *sim);

int		start_simulation(t_sim *sim, t_config *config);
int		prepare_simulation(t_sim *sim, t_config *config);
void	destroy_coder_mutexes(t_sim *sim);
void	cleanup_sim(t_sim *sim);

long	get_time_ms(void);
void	smart_sleep(t_sim *sim, long time_ms);
int		initialize_start_time(t_sim *sim);
int		check_initial_time(t_sim *sim);

void	print_status(t_coder *coder, char *status);
void	print_burnout(t_coder *coder);
void	print_compile_start(t_coder *coder);
int		all_coders_finished(t_sim *sim);
void	set_last_compile_start(t_coder *coder, long time_ms);
long	get_last_compile_start(t_coder *coder);
void	increment_compile_count(t_coder *coder);
int		get_compile_count(t_coder *coder);

void	*coder_routine(void *arg);
int		start_coder_threads(t_sim *sim);
int		join_coder_threads(t_sim *sim, int count);

void	*monitor_routine(void *arg);
int		start_monitor_thread(t_sim *sim);
int		join_monitor_thread(t_sim *sim);

int		start_threads(t_sim *sim);
int		finish_simulation(t_sim *sim);
int		handle_join_failure(t_sim *sim);

int		simulation_stopped(t_sim *sim);
void	stop_simulation(t_sim *sim);
void	wake_all_dongles(t_sim *sim);

int		dongle_request_access(t_coder *coder, t_dongle *dongle);

int		heap_init(t_heap *heap);
void	heap_destroy(t_heap *heap);
int		request_has_priority(t_request *a, t_request *b, t_policy policy);
int		heap_insert(t_heap *heap, t_request request, t_policy policy);
int		heap_top_is_coder(t_heap *heap, int coder_id);
int		heap_remove_top(t_heap *heap, t_request *out);
int		heap_remove_coder(t_heap *heap, int coder_id);

#endif
