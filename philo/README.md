*This project has been created as part of the 42 curriculum by anisabel.*

## Description

The Philosophers project is a classic implementation of the **Dining Philosophers Problem**, a fundamental problem in concurrent programming and synchronization. This project demonstrates the challenges of managing shared resources (forks) among multiple threads (philosophers) while avoiding deadlocks and ensuring fairness.

### Goal

The goal is to create a multi-threaded program where philosophers sit around a table, alternating between thinking and eating. Each philosopher needs two forks (one on each side) to eat, but forks are limited and shared. The program must:
- Simulate philosophers' actions (eating, sleeping, thinking)
- Prevent philosophers from starving to death
- Avoid deadlocks while ensuring proper resource sharing
- Track and log all philosopher actions with timestamps

### Overview

The implementation uses POSIX threads (`pthread`) and mutexes for synchronization. Each philosopher is represented by a thread that cycles through eating, sleeping, and thinking. A dedicated monitoring thread checks for philosopher deaths and whether all philosophers have satisfied their meal requirements.

## Instructions

### Compilation

To compile the project:

```bash
make
```

This will generate the executable `philo` in the project root directory.

To clean up object files:

```bash
make clean
```

To remove all generated files (object files and executable):

```bash
make fclean
```

To recompile from scratch:

```bash
make re
```

### Execution

Run the program with the following syntax:

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

**Arguments:**
- `number_of_philosophers`: The number of philosophers (and forks) at the table
- `time_to_die`: Time (in milliseconds) a philosopher has to eat before dying
- `time_to_eat`: Time (in milliseconds) it takes a philosopher to eat
- `time_to_sleep`: Time (in milliseconds) a philosopher sleeps
- `number_of_times_each_philosopher_must_eat` (optional): If specified, the simulation ends when each philosopher has eaten this many times. If omitted, the simulation runs until a philosopher dies.

**Example:**

```bash
./philo 5 800 200 200
```

This simulates 5 philosophers with 800ms to live, 200ms to eat, and 200ms to sleep.

```bash
./philo 4 410 200 200 10
```

This simulates 4 philosophers, each must eat 10 times before the simulation ends.

### Output

The program outputs timestamped logs for each philosopher's action:

```
<timestamp> <philosopher_id> <action>
```

**Actions:**
- `has taken a fork`: A philosopher picked up a fork
- `is eating`: A philosopher is eating (requires both forks)
- `is sleeping`: A philosopher is sleeping
- `is thinking`: A philosopher is thinking
- `died`: A philosopher has starved (no more output after this)

## Resources

### Classic References

1. **Dining Philosophers Problem** - Dijkstra's Classic (1971)
   - [Wikipedia - Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
   - A seminal problem in concurrent programming demonstrating resource allocation and deadlock avoidance

2. **POSIX Threads (Pthreads) Documentation**
   - [man pthread](https://man7.org/linux/man-pages/man7/pthreads.7.html)
   - Official documentation for POSIX thread library

3. **Thread Synchronization with Mutexes**
   - [man pthread_mutex_lock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
   - [man pthread_mutex_init](https://man7.org/linux/man-pages/man3/pthread_mutex_init.3p.html)

4. **Deadlock Prevention Strategies**
   - Resource hierarchy solution: Acquire resources (forks) in a consistent order
   - Banker's algorithm: A classical approach to deadlock avoidance
   - Reference: "Operating System Concepts" by Silberschatz, Galvin, and Gagne

### AI Usage

AI was used for the following tasks and parts of the project:

**Algorithm Design & Deadlock Avoidance Strategy**: AI assisted in implementing the resource hierarchy solution by ensuring philosophers acquire forks in a consistent order (even-numbered philosophers take left fork first, odd-numbered take right fork first) to prevent circular wait conditions.

## Technical Choices

- **Even/Odd Fork Acquisition**: To avoid deadlocks, philosophers with even IDs take their own fork first (left), then the next philosopher's fork (right). Odd-numbered philosophers reverse this order.
- **Smart Sleep**: The `smart_sleep` function checks if the simulation is still active during sleep periods, allowing for responsive termination.
- **Meal Tracking**: Each philosopher tracks their last meal time to detect starvation, and tracks the number of meals eaten to support the optional meal limit feature.
- **Circular Linked List**: Philosophers are organized in a circular linked list, making it easy to access the next philosopher's fork.
