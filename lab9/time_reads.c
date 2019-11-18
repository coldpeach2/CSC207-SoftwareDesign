/* The purpose of this program is to practice writing signal handling
 * functions and observing the behaviour of signals.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>


/* Message to print in the signal handling function. */
#define MESSAGE "%ld reads were done in %ld seconds.\n"

/* Global variables to store number of read operations and seconds elapsed. 
 */
long num_reads, seconds;


/* The first command-line argument is the number of seconds to set a timer to run.
 * The second argument is the name of a binary file containing 100 ints.
 * Assume both of these arguments are correct.
 */
/*Add code to set up a timer (using setitimer) for s seconds (where s is obtained from the command line). 
 Instead, use ITIMER_PROF to make the itimer send a SIGPROF. is an extra manual page that gives more advice on how to set an alarm. Part of the learning goals of this lab is practicing using a man page to learn a new system call so we are intentionally not giving you any more instruction about how to set the alarm.

Change your signal handler to print a message to standard out (use the MESSAGE format in the starter code) that provides both the total number of reads completed and the time elapsed (s seconds). Run your code a few times to see how it works.

 However, there’s more to do to explore how your computer works …*/

void handler(int code) {
  printf(MESSAGE, num_reads, seconds);
  exit(0);
}


int timer(int seconds) {
struct itimerval timer;
timer.it_interval.tv_usec = 0;
timer.it_interval.tv_sec = 0;
timer.it_value.tv_usec = 0;
timer.it_value.tv_sec = (long int) seconds;

struct itimerval prev;
int res = setitimer (ITIMER_PROF, &timer, &prev);
if (res < 0) {
  return 0; 
}
return prev.it_value.tv_sec; 
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: time_reads s filename\n");
        exit(1);
    }
    seconds = strtol(argv[1], NULL, 10);

    FILE *fp;
    if ((fp = fopen(argv[2], "r")) == NULL) {
      perror("fopen");
      exit(1);
    }

    /* In an infinite loop, read an int from a random location in the file,
     * and print it to stderr.
     */

    int fnum;
    srand(time(NULL));
    struct sigaction s;

    s.sa_handler = handler;
    s.sa_flags = 0;

    sigemptyset(&s.sa_mask);
    sigaction(SIGPROF, &s, NULL);

    timer(seconds);

    for (;;) {
      // how to randomly seek?
      int r = rand() % 100;
      fseek(fp, sizeof(int) * r, SEEK_SET);
      fread(&fnum, sizeof(int), 1, fp);
      printf("%d\n", fnum);
      num_reads ++;

    }
    return 1; // something is wrong if we ever get here!
}
