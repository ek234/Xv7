# Report

To run the code with different scheduling algorithms, we 

## Scheduling Algorithm 1: FCFS

+ For the FCFS scheduler, we add a parameter `ctime` in the struct proc which stores the creation time in ticks of the process. ctime is set in the `allocproc` function.
+ When the scheduler function is called, it calls the choose scheduler function. This function, based on the flags specified during compilation, calls the function `fcfs_scheduler`.
+ The fcfs scheduler iterates over all procs in the proc table. We acquire the lock for a proc p, check if is runnable and has the lowest creation time for a runnable process. If this is the case, it is the earliest process we have seen so far. We release the lock for the older earliest process and hold on to the key for this process. If this is not the case, i.e, we have seen a more favoured process, we release the key for p.
+ At the end of the iteration over all processes, we find get the earliest runnable process (unless no process is runnable in which case we continue). We then `swtch` to the earliest process p and release its lock.

## Scheduling Algorithm 3: LBS

+ For the LBS scheduler, we add a parameter `tickets`. This is initialized to 1 in allocproc. The tickets are inherited during fork
+ When the scheduler function is called, it calls the choose scheduler function. This function, based on the flags specified during compilation, calls the function `lbs_scheduler`.
+ First we iterate over all processes and acquire the keys of all runnable processes. We keep track of these processes in an array `acquired_procs`. We add each of their tickets to find total tickets.
+ We compute a random number less than or equal to max tickets. We then iterate over all running processes stored in `acquired_procs` and find which process the winning ticket corresponds to. All other locks are released and that process is executed by `swtch`.

## Scheduling Algorithm 3: PBS

+ For the FCFS scheduler, we add parameters `pbs_stime, pbs_rtime, num_sched, spriority`. They are initialized in allocproc.
+ pbs_stime and pbs_rtime store the number of ticks the process slept or ran for since being last scheduled.
+ num_sched stores number of times a proc has been scheduled.
+ spriority stores the static priority of the process.
+ When the scheduler function is called, it calls the choose scheduler function. This function, based on the flags specified during compilation, calls the function `pbs_scheduler`.
+ The pbs scheduler iterates over all processes in the proc table, for each process, we compute its dynamic priority based on its niceness. (If rtime and stime of a process are 0, we let its niceness be 5).
+ Based on all processes we come across, we acquire their keys. Our most favoured process is the one with the lowest dynamic priority. Tie breaks are broken with `num_sched` and `ctime`. If we come across a process that has higher priority over the most favoured process we have seen so far, we release the lock for the older process.
+ In the end we only have the lock acquired for the most favourable process. We `swtch` to this process and release its lock.

## Scheduling Algorithm 4: MLFQ

+ An array of 5 queues, qs are created as a global variable in `proc.c`. They are initialized by calling the `init_queues()` command in `main.c`.
+ When a process is created, it is added to qs[0].
+ when the mlfq_scheduler is called, we first start from the front of each queue from 0-4 and check if they require a queue promotion due to aging. If this is necessary it is done until no more procs need it.
+ We then iterate and check if any process that is runnable but not in any queue. If such a process is found, it is added in the appropriate queue.
+ We then iterate over all queues starting from queue 0 and pick the first process found. That process is executed using `swtch`
+ If the timer interrupts, we check if the current process has crossed its timeslice corresponding to the queue. If this is the case, it is yielded.