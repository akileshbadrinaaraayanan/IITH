															Minix 3.3

1.  Copy the project files into /usr/src/servers/sched

2.  cd /usr/src/minix/servers/sched
	cp <my_scheduler_name.c> schedule.c 
	This copies our scheduler to schedule.c
	<my_scheduler_name.c> can either be scheduler_orig.c, scheduler_mlfq.c, scheduler_priority.c. 

3. 	cd /usr/src/releasetools
	make hdboot
	This makes all dependencies of the scheduler by rebuilding the kernel.
	Reboot Minix 3.2.1

4.  cd /usr/src/servers/sched
	clang longrun0.c -o longrun0 
	clang mytest.c 
	./a.out > log_for_my_scheduler.txt
	This writes waiting & turnaround times of 10 longrun0 instances scheduled according to our scheduler to the logfile log_for_my_scheduler.txt
	
