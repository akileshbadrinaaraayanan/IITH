#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define LOOP_COUNT_MIN 100
#define LOOP_COUNT_MAX 100000000
int
main (int argc, char *argv[])
{
  char *idStr;
  unsigned int        v;
  int        i = 0;
  int        iteration = 1;
  int        loopCount;
  int        maxloops;
  
    //Print current Scheduler,priority, quantum
   //Set your scheduler priority.
   //Print new Scheduler,priority, quantum


  if (argc < 3 || argc > 4) {
    printf ("Usage: %s <id> <loop count> [max loops]\n", argv[0]);
    exit (-1);
  }
  /* Start with PID so result is unpredictable */
  v = getpid ();
  /* ID string is first argument */
  idStr = argv[1];
  /* loop count is second argument */
  loopCount = atoi (argv[2]);
  if ((loopCount < LOOP_COUNT_MIN) || (loopCount > LOOP_COUNT_MAX)) {
    printf ("%s: loop count must be between %d and %d (passed %d)\n",
argv[0], LOOP_COUNT_MIN, LOOP_COUNT_MAX, loopCount);
    exit (-1);
  }
  /* max loops is third argument (if present) */
  if (argc == 4) {
    maxloops = atoi (argv[3]);
  } else {
    maxloops = 0;
  }


  /* Loop forever - use CTRL-C to exit the program */
  while (1) {
    /* This calculation is done to keep the value of v unpredictable. Since
the compiler can't calculate it in advance (even from the original
value of v and the loop count), it has to do the loop. */
    v = (v << 4) - v;
    if (++i == loopCount) {
      /* Exit if we've reached the maximum number of loops. If maxloops is
0 (or negative), this'll never happen... */
      if (iteration == maxloops) {
break;
      }

      /* Feel free to change the output code to make the display clear.
         In case the display from different processes get mixed, you can use synchronization tools to make the display clear. 
      */
      printf ("Process id %d - %s:%06d\n", getpid(), idStr, iteration);		
      fflush (stdout);
      iteration += 1;
      i = 0;
    }
  }
  /* Print a value for v that's unpredictable so the compiler can't
optimize the loop away. Note that this works because the compiler
can't tell in advance that it's not an infinite loop. */
  printf ("The final value of v is 0x%08x of process %d\n", v, getpid());
}
