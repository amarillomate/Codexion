*This project has been created as part of the 42 curriculum by <andrgarc>*

# Codexion

## Description

Codexion is a concurrency simulation based on the dining philosophers problem. Each coder is a thread that loops through compiling, debugging and refactoring. To compile, a coder needs exclusive access to two shared dongles. The program implements burnout detection, a custom binary heap to arbitrate dongle requests with FIFO or EDF scheduling, dongle cooldowns and serialized logging, while avoiding deadlocks and race conditions.

## Instructions

Compile the program:

`make`

Run the simulation:

`./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <fifo|edf>`

All time arguments are expressed in milliseconds. The simulation stops when a coder burns out or when every coder has completed at least the required number of compilations.

**Clean up**:
- `make clean`: removes object files.
- `make fclean`: removes object files and the executable.
## Resources

- [POSIX threads overview](https://man7.org/linux/man-pages/man7/pthreads.7.html): thread creation, lifecycle and shared memory.
- [pthread_cond_wait](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread_cond_wait.html): mutex association, timed waits and predicate rechecking.
- [gettimeofday](https://man7.org/linux/man-pages/man2/gettimeofday.2.html): time measurement.
- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/): locks, condition variables and deadlocks.
**AI usage**: AI assistants were used to clarify POSIX synchronization patterns, review the heap and cooldown logic, and help draft the Makefile and README. All suggestions were reviewed and tested by the student.

## Blocking Cases Handled

- **Deadlocks**: coders request their dongles in ascending dongle-ID order, which breaks circular wait. If the second dongle cannot be obtained because the simulation stopped, the first one is released.
- **Race conditions**: `last_compile_start`, `compile_count`, the stop flag and dongle state are always accessed under their mutex.
- **Starvation mitigation**: each dongle has its own priority heap (FIFO or EDF), so later requests cannot overtake earlier or more urgent ones. This does not by itself guarantee that every deadline is met.
- **Burnout detection**: a separate monitor checks whether the time since a coder's last compilation start has reached `time_to_burnout`. The timestamp is refreshed when compilation begins, not when it ends.
- **Log consistency**: the second `has taken a dongle` and `is compiling` are printed together under the print mutex, with the same timestamp that is stored as `last_compile_start`. The `burned out` message and the stop flag are also set under the print mutex, so no message can appear after it.
- **Global completion**: the coder that finishes the last required compilation stops the simulation immediately, before releasing its dongles and before printing anything else. Coders that finished a few microseconds earlier may still print their normal messages.
- **Single coder**: with one coder and one dongle the coder can never compile, so it burns out without blocking the program.
- **Cooldown**: releasing a dongle sets its next availability time. A free dongle still in cooldown is waited on with `pthread_cond_timedwait()`; an occupied one with `pthread_cond_wait()`.

## Thread Synchronization Mechanisms

- **Threads** (`pthread_create`, `pthread_join`): one thread per coder plus a monitor thread. All threads are joined before mutexes and memory are destroyed.
- **Dongle mutex**: protects `in_use`, `available_at`, the request counter and the waiting heap of that dongle.
- **Coder state mutex**: protects `last_compile_start` and `compile_count`, shared between the coder and the monitor.
- **Stop mutex**: protects the `stopped` flag. On stop, all dongle conditions are broadcast to wake waiting coders.
- **Print mutex**: protects each complete log line and the stop decision for burnout. `print_status()` also checks the stop flag, so no normal messages appear after the stop. Lock order is always print mutex, then stop mutex.
- **Condition variables**: one per dongle. A coder waits until its request is at the top of the heap, the dongle is free and its cooldown has expired. The predicate is rechecked in a loop.
- **pthread_cond_broadcast**: called when a dongle is released and when the simulation stops.
- **Monitor**: it currently polls every 1 ms instead of sleeping until the nearest deadline.
