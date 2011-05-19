
# include <signal.h>
# include "testing.h"


static void handler_z09(int signal_number){

	ECHO(("handler_%d\n", signal_number));

	ECHO(("\nhandler_executed\n"));

	exit(123);
}

DEFINE_REQUIREMENT(
	z09,
	"Platform ability to handle signal SIGABRT",
	"This test sets a handler for signal <code>SIGABRT</code> and then calls function <code>abort</code> in order to verify that the handler is executed.",
	SEVERITY_CRITICAL,
	"The platform is unable to handle signal <code>SIGABRT</code>. Therefore, the library can't convert it to <code>AbortException</code>.",
	123,
	"handler_executed",
	ERROR_WHATEVER
){

	ECHO(("before_SIGNAL\n"));

	if( signal(SIGABRT, handler_z09) == SIG_ERR ){
		ECHO(("could_not_set_handler_%d\n", SIGABRT));
	}else{
		ECHO(("handler_was_set_%d\n", SIGABRT));
	}

	ECHO(("before_ABORT\n"));

	abort();

	ECHO(("after_ABORT\n"));

	return(EXIT_SUCCESS);
}
