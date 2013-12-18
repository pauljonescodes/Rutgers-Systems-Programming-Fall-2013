#include	<errno.h>
#include	<math.h>
#include	<stdlib.h>
#include	<signal.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<sys/time.h>
#include	"printBlocked.h"

// Timer demonstration of signal handling.  Timer intervals are random.

static int
bkr_random( int lo, int hi )
{
	static int	seed = 2*3*5*7*11*13*17*19+1;
	static int	prime = 2*3*5*7*11*13*17+1;
	int		value;

	value = lo + (seed>>3) % (hi-lo+1);
	seed *= prime;
	seed = seed < 0 ? -seed : seed;
	return value;
}

static void
timeout_handler( int signo, siginfo_t * info , void * p )
{
	struct itimerval	interval;

	printf( "timeout_handler() invoked.  errno is %d\n", info->si_errno );
	printBlocked( "timeout handler" );
	interval.it_interval.tv_usec = 0;				/* No repeat interval */
	interval.it_interval.tv_sec = 0;
	interval.it_value.tv_sec = 0;
	interval.it_value.tv_usec = 25000 * bkr_random( 1, 39 );	/* reset interval each time */
	setitimer( ITIMER_REAL, &interval, 0 );
}

static void
signal_handler( int signo )
{
	switch ( signo )
	{
		case SIGINT:
			printf( "Signal handler invoked.  Delivered signal is %s.\n", _sys_siglist[signo] );
			printBlocked( "signal handler" );
			exit( 0 );
			break;
		case SIGWINCH:
			system( "clear" );
			printf( "Signal handler invoked.  Window size changed.\n" );
			printBlocked( "signal handler" );
			break;
		default:
			printf( "Signal handler invoked.  unknown signal delivered is %s.\n", _sys_siglist[signo] );
			printBlocked( "signal handler" );
			break;
	}
}

int
main()
{
	struct sigaction	action;
	struct itimerval	interval;

	printf( "main() invoked in process %d.\n", getpid() );
	printBlocked( "main()" );

	action.sa_flags = SA_SIGINFO | SA_RESTART;	/* asking for long form and abilty to continue normal execution */
	action.sa_sigaction = timeout_handler;		/* long form */
	sigemptyset( &action.sa_mask );			/* no additional signals blocked */
	sigaction( SIGALRM, &action, 0 );

	action.sa_flags = 0;
	action.sa_handler = signal_handler;		/* short form */
	sigemptyset( &action.sa_mask );			/* no additional signals blocked */
	sigaction( SIGINT, &action, 0 );
	sigaction( SIGWINCH, &action, 0 );

	interval.it_interval.tv_sec = 0;				/* No repeat interval */
	interval.it_interval.tv_usec = 0;
	interval.it_value.tv_usec = 25000 * bkr_random( 1, 39 );	/* initial interval */
	interval.it_value.tv_sec = 0;
	setitimer( ITIMER_REAL, &interval, 0 );

	while( pause() == -1 );				/* wait for a signal, any signal */
	printf( "Normal end.\n" );
}
