#include	<signal.h>
#include	<stdio.h>

void
printBlocked( char * where )
{
	int		i;
	sigset_t	blockset;

	if ( sigprocmask( 0, 0, &blockset ) == -1 )
	{
		printf( "Could not get sigprocmask() for %s pid %d\n", where, getpid() );
	}
	else
	{
		printf( "Blocked signal set in %s:  [", where );
		for ( i = 1; i < 34 ; i++ )
		{
			if ( sigismember( &blockset, i ) )
				printf( " %s ", _sys_siglist[i] );
		}
		printf( "]\n" );
	}
}
