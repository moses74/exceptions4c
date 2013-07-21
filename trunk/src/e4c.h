/**
 *
 * @file        e4c.h
 *
 * exceptions4c header file
 *
 * @version     2.11
 * @author      Copyright (c) 2013 Guillermo Calvo
 *
 * @section e4c_h exceptions4c header file
 *
 * This header file needs to be included in order to be able to use any of the
 * exception handling system keywords:
 *
 * @li @c #try
 * @li @c #catch
 * @li @c #finally
 * @li @c #throw
 * @li @c #with
 * @li @c #using
 *
 * In order to stop defining these keywords, there exists a @c E4C_NOKEYWORDS
 * @e compile-time parameter. When the keywords are not defined, the next set of
 * alternative macros can be used to achieve the same functionality:
 *
 * @li @c E4C_TRY
 * @li @c E4C_CATCH
 * @li @c E4C_FINALLY
 * @li @c E4C_THROW
 * @li @c E4C_WITH
 * @li @c E4C_USING
 *
 * @section license License
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


# ifndef EXCEPTIONS4C
# define EXCEPTIONS4C


# define E4C_VERSION_(version)			version(2, 11, 1)


# if !defined(E4C_THREADSAFE) && ( \
		defined(_THREAD_SAFE) \
	||	defined(_REENTRANT) \
	||	defined(PTHREAD_H) \
	||	defined(PTHREAD_BARRIER_SERIAL_THREAD) \
	||	defined(PTHREAD_CANCEL_ASYNCHRONOUS) \
	||	defined(PTHREAD_CANCEL_ENABLE) \
	||	defined(PTHREAD_CANCEL_DEFERRED) \
	||	defined(PTHREAD_CANCEL_DISABLE) \
	||	defined(PTHREAD_CANCELED) \
	||	defined(PTHREAD_COND_INITIALIZER) \
	||	defined(PTHREAD_CREATE_DETACHED) \
	||	defined(PTHREAD_CREATE_JOINABLE) \
	||	defined(PTHREAD_EXPLICIT_SCHED) \
	||	defined(PTHREAD_INHERIT_SCHED) \
	||	defined(PTHREAD_MUTEX_DEFAULT) \
	||	defined(PTHREAD_MUTEX_ERRORCHECK) \
	||	defined(PTHREAD_MUTEX_NORMAL) \
	||	defined(PTHREAD_MUTEX_INITIALIZER) \
	||	defined(PTHREAD_MUTEX_RECURSIVE) \
	||	defined(PTHREAD_ONCE_INIT) \
	||	defined(PTHREAD_PRIO_INHERIT) \
	||	defined(PTHREAD_PRIO_NONE) \
	||	defined(PTHREAD_PRIO_PROTECT) \
	||	defined(PTHREAD_PROCESS_SHARED) \
	||	defined(PTHREAD_PROCESS_PRIVATE) \
	||	defined(PTHREAD_RWLOCK_INITIALIZER) \
	||	defined(PTHREAD_SCOPE_PROCESS) \
	||	defined(PTHREAD_SCOPE_SYSTEM) \
)
#	error "Please define E4C_THREADSAFE at compiler level " \
"in order to enable the multi-thread version of exceptions4c."
# endif


/*@-exportany@*/


/* C99 features */
# if defined(_ISOC99_SOURCE) \
	||	defined(_GNU_SOURCE) \
	||	( defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) )

#	ifndef HAVE_C99_STDBOOL
#		define HAVE_C99_STDBOOL
#	endif

#	ifndef HAVE_C99_VARIADIC_MACROS
#		define HAVE_C99_VARIADIC_MACROS
#	endif

#	ifndef HAVE_C99_FUNC
#		define HAVE_C99_FUNC
#	endif

#	ifndef HAVE_C99_VSNPRINTF
#		define HAVE_C99_VSNPRINTF
#	endif

#	ifndef HAVE_C99_SNPRINTF
#		define HAVE_C99_SNPRINTF
#	endif

# endif


/* POSIX features */
# if defined(_POSIX_C_SOURCE) \
	||	defined(_POSIX_SOURCE) \
	||	defined(_POSIX_VERSION) \
	||	defined(_POSIX2_C_VERSION) \
	||	defined(_XOPEN_SOURCE) \
	||	defined(_XOPEN_VERSION) \
	||	defined(_XOPEN_SOURCE_EXTENDED) \
	||	defined(_GNU_SOURCE)

/*
 * POSIX.1 does not specify whether setjmp and longjmp save or restore the
 * current set of blocked signals. If a program employs signal handling it
 * should use POSIX's sigsetjmp/siglongjmp.
 */
#	ifndef HAVE_POSIX_SIGSETJMP
#		define HAVE_POSIX_SIGSETJMP
#	endif

# endif


# include <stdlib.h>
# include <setjmp.h>


# if defined(HAVE_C99_STDBOOL) || defined(HAVE_STDBOOL_H)
#	include <stdbool.h>
# endif

# if	defined(__bool_true_false_are_defined) \
	||	defined(bool) \
	||	defined(S_SPLINT_S)
#	define E4C_BOOL						bool
#	define E4C_FALSE					false
#	define E4C_TRUE						true
# else
#	define E4C_BOOL						int
#	define E4C_FALSE					0
#	define E4C_TRUE						1
# endif


/*
 * Make sure we can use exceptions4c within C++.
 */
# ifdef __cplusplus
	extern "C" {
# endif


/*
 * The E4C_FUNCTION_NAME_ compile-time parameter
 * could be defined in order to work with some specific compiler.
 */
# ifndef E4C_FUNCTION_NAME_

#	ifdef HAVE_C99_FUNC
#		define E4C_FUNCTION_NAME_		__func__

#	elif defined(__GNUC__)
#		if !defined(__OPTIMIZE__) && (__GNUC__ >= 2)
#			define E4C_FUNCTION_NAME_	__extension__ __FUNCTION__
#		else
#			define E4C_FUNCTION_NAME_	NULL
#		endif

#	else
#		define E4C_FUNCTION_NAME_		NULL
#	endif

# endif


/*
 * The E4C_INVALID_SIGNAL_NUMBER_ compile-time parameter
 * could be defined in order to work with some specific compiler.
 */
# ifndef E4C_INVALID_SIGNAL_NUMBER_

#	define E4C_INVALID_SIGNAL_NUMBER_	-1

# endif


/*
 * The E4C_NO_RETURN_ compile-time parameter
 * could be defined in order to work with some specific compiler.
 */
# ifdef E4C_NO_RETURN_
#	define E4C_UNREACHABLE_RETURN_(_value_)		( (void)0 )
#	define E4C_UNREACHABLE_VOID_RETURN_			( (void)0 )

# elif defined(__GNUC__)
#	define E4C_NO_RETURN_						__attribute__ ((noreturn))
#	define E4C_UNREACHABLE_RETURN_(_value_)		( (void)0 )
#	define E4C_UNREACHABLE_VOID_RETURN_			( (void)0 )

# elif defined(S_SPLINT_S)
#	define E4C_NO_RETURN_
#	define E4C_UNREACHABLE_RETURN_(_value_) \
		/*@-unreachable@*/ /*@-noeffect@*/ \
		( (void)0 ) \
		/*@=unreachable@*/ /*@=noeffect@*/
#	define E4C_UNREACHABLE_VOID_RETURN_ \
		/*@-unreachable@*/ /*@-noeffect@*/ \
		( (void)0 ) \
		/*@=unreachable@*/ /*@=noeffect@*/

# else
#	define E4C_NO_RETURN_
#	define E4C_UNREACHABLE_RETURN_(_value_)		return(_value_)
#	define E4C_UNREACHABLE_VOID_RETURN_			return

# endif


# if defined(HAVE_POSIX_SIGSETJMP) || defined(HAVE_SIGSETJMP)
#	define E4C_CONTINUATION_BUFFER_		sigjmp_buf
#	define E4C_CONTINUATION_CREATE_(_continuation_) \
		(void)sigsetjmp(_continuation_->buffer, 1)
# else
#	define E4C_CONTINUATION_BUFFER_		jmp_buf
#	define E4C_CONTINUATION_CREATE_(_continuation_) \
		(void)setjmp(_continuation_->buffer)
# endif


# ifndef NDEBUG
#	define E4C_INFO_FILE_				__FILE__
#	define E4C_INFO_LINE_				__LINE__
#	define E4C_INFO_FUNC_				E4C_FUNCTION_NAME_
#	define E4C_ASSERT(_condition_) ( \
		(_condition_) \
		? (void)0 \
		: E4C_THROW(AssertionException, "Assertion failed: " #_condition_) \
	)
# else
#	define E4C_INFO_FILE_				NULL
#	define E4C_INFO_LINE_				0
#	define E4C_INFO_FUNC_				NULL
#	define E4C_ASSERT(_ignore_)			( (void)0 )
# endif

# define E4C_INFO_ \
			E4C_INFO_FILE_, \
			E4C_INFO_LINE_, \
			E4C_INFO_FUNC_


# define E4C_PASTE_(_x_, _y_, _z_)		_x_ ## _ ## _y_ ## _ ## _z_
# define E4C_MANGLE_(_pre_, _id_, _post_) E4C_PASTE_(_pre_, _id_, _post_)
# define E4C_AUTO_(_id_)				E4C_MANGLE_(_implicit, _id_, __LINE__)


# ifdef E4C_THREADSAFE
#	define E4C_VERSION_THREADSAFE_			1
#	define E4C_VERSION_THREADSAFE_STRING_	" (multi-thread)"
# else
#	define E4C_VERSION_THREADSAFE_			0
#	define E4C_VERSION_THREADSAFE_STRING_	" (single-thread)"
# endif


# define E4C_VERSION_STRING_(_major_, _minor_, _revision_) \
	#_major_ "." #_minor_ "." #_revision_ E4C_VERSION_THREADSAFE_STRING_
# define E4C_VERSION_NUMBER_(_major_, _minor_, _revision_) ( \
	( (long)E4C_VERSION_THREADSAFE_	* 10000000L) +	\
	( (long)_major_					* 1000000L) +	\
	( (long)_minor_					* 1000L) +		\
	( (long)_revision_				* 1L)			\
)
# define E4C_VERSION_MAJOR_(_major_, _minor_, _revision_) ( (int)_major_ )
# define E4C_VERSION_MINOR_(_major_, _minor_, _revision_) ( (int)_minor_ )
# define E4C_VERSION_REVISION_(_major_, _minor_, _revision_) ( (int)_revision_ )


/*
 * These undocumented macros hide implementation details from documentation.
 */

# define E4C_FRAME_LOOP_(_stage_) \
	E4C_CONTINUATION_CREATE_( e4c_frame_first_stage_(_stage_, E4C_INFO_) ); \
	while( e4c_frame_next_stage_() )

# define E4C_TRY \
	E4C_FRAME_LOOP_(e4c_acquiring_) \
	if( ( e4c_frame_get_stage_(E4C_INFO_) == e4c_trying_ ) \
		&& e4c_frame_next_stage_() )
	/* simple optimization: e4c_frame_next_stage_ will avoid disposing stage */

# define E4C_CATCH(_exception_type_) \
	else if( e4c_frame_catch_(&_exception_type_, E4C_INFO_) )

# define E4C_FINALLY \
	else if( e4c_frame_get_stage_(E4C_INFO_) == e4c_finalizing_ )

# define E4C_THROW(_exception_type_, _message_) \
	e4c_exception_throw_verbatim_(&_exception_type_, E4C_INFO_, _message_ )

# define E4C_WITH(_resource_, _dispose_) \
	E4C_FRAME_LOOP_(e4c_beginning_) \
	if( e4c_frame_get_stage_(E4C_INFO_) == e4c_disposing_ ){ \
		_dispose_( \
			/*@-usedef@*/ (_resource_) /*@=usedef@*/, \
			(e4c_get_status() == e4c_failed) \
		); \
	}else if( e4c_frame_get_stage_(E4C_INFO_) == e4c_acquiring_ ){
	/*
	 * Splint detects _resource_ being used before it is defined,
	 * but we *really* do define it before using, so we need to
	 * disable this check (usedef) for this specific parameter.
	 */

# define E4C_USE \
	}else if( e4c_frame_get_stage_(E4C_INFO_) == e4c_trying_ )

# define E4C_USING(_type_, _resource_, _args_) \
	E4C_WITH( (_resource_), e4c_dispose_##_type_){ \
		(_resource_) = e4c_acquire_##_type_ _args_; \
	}E4C_USE

# define E4C_REUSING_CONTEXT(_status_, _on_failure_) \
	\
	volatile E4C_BOOL		E4C_AUTO_(BEGIN)	= !e4c_context_is_ready(); \
	volatile E4C_BOOL		E4C_AUTO_(DONE)		= E4C_FALSE; \
	\
	if( E4C_AUTO_(BEGIN) ){ \
		e4c_context_begin(E4C_FALSE); \
		E4C_TRY{ \
			goto E4C_AUTO_(PAYLOAD); \
			E4C_AUTO_(CLEANUP): \
			E4C_AUTO_(DONE) = E4C_TRUE; \
		}E4C_CATCH(RuntimeException){ \
			(_status_) = (_on_failure_); \
		} \
		e4c_context_end(); \
		E4C_AUTO_(DONE)		= E4C_TRUE; \
		E4C_AUTO_(BEGIN)	= E4C_FALSE; \
	} \
	\
	E4C_AUTO_(PAYLOAD): \
	for(; !E4C_AUTO_(DONE) || E4C_AUTO_(BEGIN); E4C_AUTO_(DONE) = E4C_TRUE) \
		if( E4C_AUTO_(DONE) ){ \
			goto E4C_AUTO_(CLEANUP); \
		}else

# define E4C_USING_CONTEXT(_handle_signals_) \
	\
	for( \
		e4c_context_begin(_handle_signals_); \
		e4c_context_is_ready(); \
		e4c_context_end() \
	)

# ifdef HAVE_C99_VARIADIC_MACROS
#	define E4C_THROWF(_exception_type_, _format_, ...) \
		e4c_exception_throw_format_( \
			&_exception_type_, E4C_INFO_, _format_, __VA_ARGS__ \
		)
# endif

# define E4C_RETHROW(_message_) \
	e4c_exception_throw_verbatim_( \
		( \
			e4c_get_exception() == NULL \
			? &NullPointerException \
			: e4c_get_exception()->type \
		), \
		E4C_INFO_, _message_ \
	)

# ifdef HAVE_C99_VARIADIC_MACROS
#	define E4C_RETHROWF(_format_, ...) \
		e4c_exception_throw_format_( \
			( e4c_get_exception() == NULL ? NULL : e4c_get_exception()->type), \
			E4C_INFO_, _format_, __VA_ARGS__ \
		)
# endif

# define E4C_RETRY(_max_retry_attempts_) \
	e4c_frame_repeat_(_max_retry_attempts_, e4c_acquiring_, E4C_INFO_)

# define E4C_REACQUIRE(_max_reacquire_attempts_) \
	e4c_frame_repeat_(_max_reacquire_attempts_, e4c_beginning_, E4C_INFO_)


/**
 * @name Exception handling keywords
 *
 * This set of keywords express the semantics of exception handling.
 *
 * @{
 */

/**
 * Introduces a block of code aware of exceptions
 *
 * A @c #try statement executes a block of code. If an exception is thrown and
 * there is a @c #catch block that can handle it, then control will be
 * transferred to it. If there is a @c #finally block, then it will be executed,
 * no matter whether the @c #try block completes normally or abruptly, and no
 * matter whether a @c #catch block is first given control.
 *
 * The block of code immediately after the keyword @c #try is called <strong>the
 * @c #try block</strong> of the @c #try statement. The block of code
 * immediately after the keyword @c #finally is called <strong>the @c #finally
 * block</strong> of the @c #try statement.
 *
 * @code
 * stack_t * stack = stack_new();
 * try{
 *     // the try block
 *     int value = stack_pop(stack);
 *     stack_push(stack, 16);
 *     stack_push(stack, 32);
 * }catch(StackOverflowException){
 *     // a catch block
 *     printf("Could not push.");
 * }catch(StackUnderflowException){
 *     // another catch block
 *     printf("Could not pop.");
 * }finally{
 *     // the finally block
 *     stack_delete(stack);
 * }
 * @endcode
 *
 * One @c #try block may precede many @c #catch blocks (also called
 * <em>exception handlers</em>). A @c #catch block @b must have exactly one
 * parameter, which is the exception type it is capable of handling. Within the
 * @c #catch block, the exception can be accessed through the function
 * @c #e4c_get_exception. Exception handlers are considered in left-to-right
 * order: the earliest possible @c #catch block handles the exception. If no
 * @c #catch block can handle the thrown exception, it will be @e propagated.
 *
 * Sometimes it can come in handy to #retry an entire @c #try block, for
 * instance, once the exception has been caught and the error condition has been
 * solved.
 *
 * A @c #try block has an associated @e status according to the way it has been
 * executed:
 *
 * @li It @e succeeds when the execution reaches the end of the block without
 *          any exceptions.
 * @li It @e recovers when an exception is thrown but a @c #catch block handles
 *          it.
 * @li It @e fails when an exception is thrown and it's not caught.
 *
 * The status of the current @c #try block can be retrieved through the function
 * @c #e4c_get_status.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #try. Such programming error will lead to an
 *          abrupt exit of the program (or thread).
 * @pre     A @c #try block @b must precede, at least, another block of code,
 *          introduced by either @c #catch or @c #finally.
 * @pre     A @c #try block may precede several @c #catch blocks.
 * @pre     A @c #try block @e can precede, at most, one @c #finally block.
 * @pre     A @c #try block <strong>must not</strong> be exited through any of:
 *          @c goto, @c break, @c continue or @c return (but it is legal to
 *          @c #throw an exception).
 *
 * @post    A @c #finally block will be executed after the @c #try block and any
 *          @c #catch block that might be executed, no matter whether the
 *          @c #try block @e succeeds, @e recovers or @e fails.
 *
 * @see     #catch
 * @see     #finally
 * @see     #retry
 * @see     #e4c_status
 * @see     #e4c_get_status
 */
# ifndef E4C_NOKEYWORDS
# define try E4C_TRY
# endif

/**
 * Introduces a block of code capable of handling a specific type of exceptions
 *
 * @param   _exception_type_
 *          The type of exceptions to be handled
 *
 * @c #catch blocks are optional code blocks that @b must be preceded by
 * @c #try, <code>#with... #use</code> or @c #using blocks. Several @c #catch
 * blocks can be placed next to one another.
 *
 * When an exception is thrown, the system looks for a @c #catch block to handle
 * it. The first capable block (in order of appearance) will be executed and the
 * exception is said to be @e caught.
 *
 * The caught exception can be accessed through the function #e4c_get_exception.
 *
 * @code
 * try{
 *     ...
 * }catch(RuntimeException){
 *     const e4c_exception * exception = e4c_get_exception();
 *     printf("Error: %s", exception->message);
 * }
 * @endcode
 *
 * The actual @c type of the exception can be checked against other exception
 * types through the function #e4c_is_instance_of.
 *
 * @code
 * try{
 *     ...
 * }catch(RuntimeException){
 *     const e4c_exception * exception = e4c_get_exception();
 *     if( e4c_is_instance_of(exception, &SignalException) ){
 *         // the exception type is SignalException or any subtype
 *     }
 * }
 * @endcode
 *
 * The @c type might also be compared directly against another specific
 * exception type.
 *
 * @code
 * try{
 *     ...
 * }catch(RuntimeException){
 *     const e4c_exception * exception = e4c_get_exception();
 *     if(exception->type == &NotEnoughMemoryException){
 *         // the exception type is precisely NotEnoughMemoryException
 *     }
 * }
 * @endcode
 *
 * After the @c #catch block completes, the @c #finally block (if any) is
 * executed. Then the program continues by the next line following the set of
 * @c #try... @c #catch... @c #finally blocks.
 *
 * However, if an exception is thrown in a @c #catch block, then the @c #finally
 * block will be executed right away and the system will look for an outter
 * @c #catch block to handle it.
 *
 * Only one of all the @c #catch blocks will be executed for each @c #try block,
 * even though the executed @c #catch block throws another exception. The only
 * possible way to execute more than one @c #catch block would be by
 * <code>#retry</code>ing the entire @c #try block.
 *
 * @pre     A @c #catch block @b must be preceded by one of these blocks:
 *          @li A @c #try block
 *          @li A @c #with... @c #use block
 *          @li A @c #using block
 *          @li Another @c #catch block.
 * @pre     A @c #catch block <strong>must not</strong> be exited through any
 *          of: @c goto, @c break, @c continue or @c return (but it is legal to
 *          @c #throw an exception).
 *
 * @see     #try
 * @see     #e4c_exception_type
 * @see     #e4c_get_exception
 * @see     #e4c_exception
 * @see     #e4c_is_instance_of
 */
# ifndef E4C_NOKEYWORDS
# define catch(_exception_type_) E4C_CATCH(_exception_type_)
# endif

/**
 * Introduces a block of code responsible for cleaning up the previous
 * exception-aware block
 *
 * @c #finally blocks are optional code blocks that @b must be preceded by
 * @c #try, @c #with... @c #use or @c #using blocks. It is allowed to place,
 * at most, one @c #finally block.
 *
 * The @c #finally block can determine the completeness of the
 * @e exception-aware block through the function @c #e4c_get_status. The thrown
 * exception (if any) can also be accessed through the function
 * @c #e4c_get_exception.
 *
 * @code
 * try{
 *    ...
 * }finally{
 *    switch( e4c_get_status() ){
 *
 *        case e4c_succeeded:
 *            ...
 *            break;
 *
 *        case e4c_recovered:
 *            ...
 *            break;
 *
 *        case e4c_failed:
 *            ...
 *            break;
 *    }
 * }
 * @endcode
 *
 * The finally block will be executed only @b once. The only possible way to be
 * executed again would be by <code>#retry</code>ing the entire @c #try block.
 *
 * @pre     A @c #finally block @b must be preceded by a @c #try, @c #with...
 *          @c #use, @c #using or @c #catch block.
 * @pre     A @c #finally block <strong>must not</strong> be exited through any
 *          of: @c goto, @c break, @c continue or @c return (but it is legal to
 *          @c #throw an exception).
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #finally. Such programming error will lead to
 *          an abrupt exit of the program (or thread).
 *
 * @see     #e4c_exception
 * @see     #e4c_get_exception
 * @see     #e4c_get_status
 * @see     #e4c_status
 */
# ifndef E4C_NOKEYWORDS
# define finally E4C_FINALLY
# endif

/**
 * Repeats the previous @c #try (or @c #use) block entirely
 *
 * @param   _max_retry_attempts_
 *          The maximum number of attempts to retry
 *
 * This macro discards any thrown exception (if any) and repeats the previous
 * @c #try or @c #use block, up to a specified maximum number of attempts.
 *
 * This macro is intended to be used within @c #catch or @c #finally blocks as a
 * quick way to fix an error condition and <em>try again</em>.
 *
 * @code
 * const char * file_path = config_get_user_defined_file_path();
 * try{
 *     config = read_config(file_path);
 * }catch(ConfigException){
 *     file_path = config_get_default_file_path();
 *     retry(1);
 *     rethrow("Wrong defaults.");
 * }
 * @endcode
 *
 * @warning
 * If the specified maximum number of attempts is zero, then the block can
 * eventually be attempted an unlimited number of times. Care should be taken in
 * order not to create an <em>infinite loop</em>.
 *
 * This macro won't return control unless the block has already been attempted,
 * at least, the specified maximum number of times.
 *
 * @note
 * At a @c #catch block, the current exception is considered caught, whether the
 * @c #retry takes place or not. If you want the exception to be propagated when
 * the maximum number of attempts has been reached, then you need to @c #rethrow
 * it again.
 *
 * @code
 * int dividend = 100;
 * int divisor = 0;
 * int result = 0;
 * try{
 *     result = dividend / divisor;
 *     do_something(result);
 * }catch(RuntimeException){
 *     divisor = 1;
 *     retry(1);
 *     rethrow("Error (not a division by zero).");
 * }
 * @endcode
 *
 * @note
 * At a @c #finally block, the current exception (if any) will be propagated if
 * the @c #retry does not take place, so you don't need to @c #rethrow it again.
 *
 * @code
 * int dividend = 100;
 * int divisor = 0;
 * int result = 0;
 * try{
 *     result = dividend / divisor;
 *     do_something(result);
 * }finally{
 *     if( e4c_get_status() == e4c_failed ){
 *         divisor = 1;
 *         retry(1);
 *         // when we get here, the exception will be propagated
 *     }
 * }
 * @endcode
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #retry. Such programming error will lead to an
 *          abrupt exit of the program (or thread).
 * @pre     The @c #retry keyword @b must be used from a @c #catch or
 *          @c #finally block.
 * @post    Control does not return to the @c #retry point, unless the @c #try
 *          (or @c #use) block has been attempted, at least, the specified
 *          number of times.
 *
 * @see     #reacquire
 * @see     #try
 * @see     #use
 */
# ifndef E4C_NOKEYWORDS
#	define retry(_max_retry_attempts_) E4C_RETRY(_max_retry_attempts_)
# endif

/**
 * Signals an exceptional situation represented by an exception object
 *
 * @param   _exception_type_
 *          The type of exception to be thrown
 * @param   _message_
 *          The @e ad-hoc message describing the exception. If @c NULL, then the
 *          default message for the specified exception type will be used
 *
 * Creates a new instance of the specified type of exception and then throws it.
 * The provided message is copied into the thrown exception, so it can be freely
 * deallocated. If @c NULL is passed, then the default message for that type of
 * exception will be used.
 *
 * When an exception is thrown, the exception handling framework looks for the
 * appropriate @c #catch block that can handle the exception. The system unwinds
 * the call chain of the program and executes the @c #finally blocks it finds.
 *
 * When no @c #catch block is able to handle an exception, the system eventually
 * gets to the main function of the program. This situation is called an
 * <strong><em>uncaught exception</em></strong>.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #throw. Such programming error will lead to an
 *          abrupt exit of the program (or thread).
 * @post    Control does not return to the @c #throw point.
 *
 * @see     #throwf
 * @see     #rethrow
 * @see     #e4c_exception_type
 * @see     #e4c_exception
 * @see     #e4c_uncaught_handler
 * @see     #e4c_get_exception
 */
# ifndef E4C_NOKEYWORDS
# define throw(_exception_type_, _message_) \
	E4C_THROW(_exception_type_, _message_)
# endif

/**
 * Throws again the currently thrown exception, with a new message
 *
 * @param   _message_
 *          The new message describing the exception. It should be more specific
 *          than the current one
 *
 * This macro creates a new instance of the thrown exception, with a more
 * specific message.
 *
 * @c #rethrow is intended to be used in a @c #catch block and the purpose is to
 * refine the message of the currently caught exception. The previous exception
 * (and its message) will be stored as the @e cause of the newly thrown
 * exception.
 *
 * @code
 * try{
 *     image = read_file(file_path);
 * }catch(FileOpenException){
 *     rethrow("Image not found.");
 * }
 * @endcode
 *
 * The semantics of this keyword are the same as for @c #throw.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #rethrow. Such programming error will lead to
 *          an abrupt exit of the program (or thread).
 * @post    Control does not return to the @c #rethrow point.
 *
 * @see     #throw
 * @see     #rethrowf
 */
# ifndef E4C_NOKEYWORDS
#	define rethrow(_message_) E4C_RETHROW(_message_)
# endif

/** @} */

/**
 * @name Dispose pattern keywords
 *
 * This set of keywords express the semantics of automatic resource acquisition
 * and disposal.
 *
 * @{
 */

/**
 * Opens a block of code with automatic disposal of a resource
 *
 * @param   _resource_
 *          The resource to be disposed
 * @param   _dispose_
 *          The name of the disposal function (or macro)
 *
 * The combination of keywords @c #with... @c #use encapsules the <em>Dispose
 * Pattern</em>. This pattern consists of two separate blocks and an implicit
 * call to a given function:
 *
 * <ol>
 *      <li>the @c #with block is responsible for the resource acquisition</li>
 *      <li>the @c #use block makes use of the resource</li>
 *      <li>the disposal function will be called implicitly</li>
 * </ol>
 *
 * A @c #with block @b must be followed by a @c #use block. In addition, the
 * @c #use block may be followed by @c #catch and @c #finally blocks.
 *
 * The @c #with keyword guarantees that the disposal function will be called
 * <strong>if, and only if,</strong> the acquisition block @e completed without
 * an error (i.e. no exception being thrown from the acquisition block).
 *
 * If the @c #with block does not complete, neither the disposal function nor
 * the @c #use block will be executed.
 *
 * The disposal function is called right after the @c #use block. If an
 * exception was thrown, the @c #catch  or @c #finally blocks (if any) will take
 * place @b after the disposal of the resource.
 *
 * When called, the disposal function will receive two arguments:
 *
 * @li The resource
 * @li A boolean flag indicating if the @c #use block did not @e complete
 *
 * This way, different actions can be taken depending on the success or failure
 * of the block. For example, commiting or rolling back a @e transaction
 * resource.
 *
 * Legacy functions can be reused by defining macros. For example, a file
 * resource needs to be closed regardless of the errors. Since the function
 * @c fclose only takes one parameter, we could define the next macro:
 *
 * @code
 * # define e4c_dispose_file(_file_, _failed_) fclose(_file_)
 * @endcode
 *
 * Here is the typical usage of @c #with... @c #use:
 *
 * @code
 * with(foo, e4c_dispose_foo){
 *     foo = e4c_acquire_foo(foobar);
 *     someAssertion(foo, bar);
 *     ...
 * }use{
 *     doSomething(foo);
 *     ...
 * }catch(FooAcquisitionFailed){
 *     recover1();
 *     ...
 * }catch(somethingElseFailed){
 *     recover2();
 *     ...
 * }finally{
 *     cleanup();
 *     ...
 * }
 * @endcode
 *
 * Nonetheless, one-liners fit nicely too:
 *
 * @code
 * with(foo, e4c_dispose_foo) foo = e4c_acquire_foo(bar, foobar); use doSomething(foo);
 * @endcode
 *
 * There is a way to lighten up even more this pattern by defining convenience
 * macros, customized for a specific kind of resources. For example,
 * @c e4c_using_file or @c e4c_using_memory.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #with. Such programming error will lead to an
 *          abrupt exit of the program (or thread).
 * @pre     A @c #with block <strong>must not</strong> be exited through any of:
 *          @c goto, @c break, @c continue or @c return (but it is legal to
 *          @c #throw an exception).
 * @pre     A @c #with block @b must always be followed by a @c #use block.
 *
 * @see     #use
 * @see     #using
 */
# ifndef E4C_NOKEYWORDS
# define with(_resource_, _dispose_) E4C_WITH(_resource_, _dispose_)
# endif

/**
 * Closes a block of code with automatic disposal of a resource
 *
 * A @c #use block @b must always be preceded by a @c #with block. These two
 * keywords are designed so the compiler will complain about @e dangling
 * @c #with... @c #use blocks.
 *
 * A code block introduced by the @c #use keyword will only be executed when
 * (and if) the acquisition of the resource @e completes without exceptions.
 *
 * Either if the @c #use block completes or not, the disposal function will be
 * executed right away.
 *
 * @pre     A @c #use block @b must be preceded by a @c #with block.
 * @pre     A @c #use block <strong>must not</strong> be exited through any of:
 *          @c goto, @c break, @c continue or @c return (but it is legal to
 *          @c #throw an exception).
 *
 * @see     #with
 */
# ifndef E4C_NOKEYWORDS
# define use E4C_USE
# endif

/**
 * Introduces a block of code with automatic acquisition and disposal of a
 * resource
 *
 * @param   _type_
 *          The type of the resource
 * @param   _resource_
 *          The resource to be acquired, used and then disposed
 * @param   _args_
 *          A list of arguments to be passed to the acquisition function
 *
 * The specified resource will be acquired, used and then disposed. The
 * automatic acquisition and disposal is achieved by calling the @e magic
 * functions:
 *
 * @li <code><em>_type_</em> <strong>e4c_acquire_<em>_type_</em></strong>(_args_)</code>
 * @li <code>void <strong>e4c_dispose_<em>_type_</em></strong>(<em>_type_</em> _resource_, E4C_BOOL _failed_)</code>
 *
 * The resource will be acquired implicitly by assigning to it the result of the
 * @e magic acquisition function
 * <code><strong>e4c_acquire_<em>_type_</em></strong></code>.
 *
 * The semantics of the automatic acquisition and disposal are the same as for
 * blocks introduced by @c #with... @c #use. For example, a @c #using block
 * can also precede @c #catch and @c #finally blocks.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #using. Such programming error will lead to an
 *          abrupt exit of the program (or thread).
 * @pre     A @c #using block <strong>must not</strong> be exited through any
 *          of: @c goto, @c break, @c continue or @c return (but it is legal to
 *          @c #throw an exception).
 *
 * @see     #with
 */
# ifndef E4C_NOKEYWORDS
# define using(_type_, _resource_, _args_) E4C_USING(_type_, _resource_, _args_)
# endif

/**
 * Repeats the previous @c #with block entirely
 *
 * @param   _max_reacquire_attempts_
 *          The maximum number of attempts to reacquire
 *
 * This macro discards any thrown exception (if any) and repeats the previous
 * @c #with block, up to a specified maximum number of attempts. If the
 * acquisition completes, then the @c #use block will be executed.
 *
 * It is intended to be used in @c #catch or @c #finally blocks, next to a
 * @c #with... @c #use or @c #using block when the resource acquisition failed,
 * as a quick way to fix an error condition and try to acquire the resource
 * again.
 *
 * @code
 * image_type * image;
 * const char * image_path = image_get_user_avatar();
 * with(image, e4c_image_dispose){
 *     image = e4c_image_acquire(image_path);
 * }use{
 *     image_show(image);
 * }catch(ImageNotFoundException){
 *     image_path = image_get_default_avatar();
 *     reacquire(1);
 * }
 * @endcode
 *
 * @warning
 * If the specified maximum number of attempts is zero, then the @c #with block
 * can eventually be attempted an unlimited number of times. Care should be
 * taken in order not to create an <em>infinite loop</em>.
 *
 * Once the resource has been acquired, the @c #use block can also be repeated
 * @e alone through the keyword @c #retry:
 *
 * @code
 * image_type * image;
 * const char * image_path = image_get_user_avatar();
 * display_type * display = display_get_user_screen();
 * with(image, e4c_image_dispose){
 *     image = e4c_image_acquire(image_path);
 * }use{
 *     image_show(image, display);
 * }catch(ImageNotFoundException){
 *     image_path = image_get_default_avatar();
 *     reacquire(1);
 * }catch(DisplayException){
 *     display = display_get_default_screen();
 *     retry(1);
 * }
 * @endcode
 *
 * @pre     The @c #reacquire keyword @b must be used from a @c #catch or
 *          @c #finally block, preceded by @c #with... @c #use or @c #using
 *          blocks.
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #reacquire. Such programming error will lead to
 *          an abrupt exit of the program (or thread).
 * @post    This macro won't return control unless the @c #with block has
 *          already been attempted, at least, the specified maximum number of
 *          times.
 *
 * @see     #retry
 * @see     #with
 * @see     #use
 */
# ifndef E4C_NOKEYWORDS
#	define reacquire(_max_reacquire_attempts_) \
		E4C_REACQUIRE(_max_reacquire_attempts_)
# endif

/** @} */

/**
 * @name Integration macros
 *
 * These macros are designed to ease the integration of external libraries which
 * make use of the exception handling system.
 *
 * @{
 */

/**
 * Provides the library version number
 *
 * The library version number is a @c long value which expresses:
 *
 * @li library thread mode (either @e single-thread or @e multi-thread)
 * @li library @e major version number
 * @li library @e minor version number
 * @li library @e revision number
 *
 * The multi-thread (or @e thread-safe) mode can be obtained by compiling the
 * library with the @c E4C_THREADSAFE @e compile-time parameter.
 *
 * The formula to encode these version numbers into a single @c long value is:
 *
 * @code
 * THREADSAFE * 10000000 + MAJOR * 1000000 + MINOR * 1000 + REVISION
 * @endcode
 *
 * These numbers can be obtained separately through the next macros:
 *
 * @li @c #E4C_VERSION_THREADSAFE
 * @li @c #E4C_VERSION_MAJOR
 * @li @c #E4C_VERSION_MINOR
 * @li @c #E4C_VERSION_REVISION
 *
 * The library version number can be also obtained as a string literal in the
 * format "MAJOR.MINOR.REVISION (THREADSAFE)" through the macro
 * @c #E4C_VERSION_STRING.
 *
 * @note
 * This version number can be considered as the @e compile-time library version
 * number, as opposed to the @e run-time library version number (associated with
 * the actual, compiled library). This @e run-time version number can be
 * obtained through the function @c #e4c_library_version.
 *
 * @remark
 * The library @b must be compiled with the corresponding header (i.e. library
 * version number should be equal to header version number).
 *
 * @see     #e4c_library_version
 * @see     #E4C_VERSION_THREADSAFE
 * @see     #E4C_VERSION_MAJOR
 * @see     #E4C_VERSION_MINOR
 * @see     #E4C_VERSION_REVISION
 * @see     #E4C_VERSION_STRING
 */
# define E4C_VERSION_NUMBER \
	\
	E4C_VERSION_(E4C_VERSION_NUMBER_)

/**
 * Provides the library thread mode (either single-thread or multi-thread)
 *
 * When the library is compiled with the @c E4C_THREADSAFE @e compile-time
 * parameter, @c #E4C_VERSION_THREADSAFE will yield the @c int value @c 1
 * (meaning @e multi-thread mode), otherwise it will yield the @c int value @c 0
 * (meaning @e single-thread mode).
 *
 * @see     #E4C_VERSION_NUMBER
 */
# define E4C_VERSION_THREADSAFE \
	\
	E4C_VERSION_THREADSAFE_

/**
 * Provides the library major version number
 *
 * The library major version number is an @c int value which is incremented from
 * one release to another when there are <strong>significant changes in
 * functionality</strong>.
 *
 * @see     #E4C_VERSION_NUMBER
 */
# define E4C_VERSION_MAJOR \
	\
	E4C_VERSION_(E4C_VERSION_MAJOR_)

/**
 * Provides the library minor version number
 *
 * The library minor version number is an @c int value which is incremented from
 * one release to another when <strong>only minor features or significant fixes
 * have been added</strong>.
 *
 * @see     #E4C_VERSION_NUMBER
 */
# define E4C_VERSION_MINOR \
	\
	E4C_VERSION_(E4C_VERSION_MINOR_)

/**
 * Provides the library revision number
 *
 * The library revision number is an @c int value which is incremented from one
 * release to another when <strong>minor bugs are fixed</strong>.
 *
 * @see     #E4C_VERSION_NUMBER
 */
# define E4C_VERSION_REVISION \
	\
	E4C_VERSION_(E4C_VERSION_REVISION_)

/**
 * Provides the library version number as a string literal
 *
 * The format of the string literal is: "MAJOR.MINOR.REVISION (THREADSAFE)".
 *
 * @see     #E4C_VERSION_NUMBER
 */
# define E4C_VERSION_STRING \
	\
	E4C_VERSION_(E4C_VERSION_STRING_)

/**
 * Provides the maximum length (in bytes) of an exception message
 */
# define E4C_EXCEPTION_MESSAGE_SIZE 128

/**
 * Reuses an existing exception context, otherwise, begins a new one and then
 * ends it.
 *
 * @param   _status_
 *          The name of a previously defined variable, or lvalue, which will be
 *          assigned the specified failure value
 * @param   _on_failure_
 *          A constant value or expression that will be assigned to the
 *          specified lvalue in case of failure
 *
 * This macro lets library developers use the exception framework, regardless of
 * whether the library clients are unaware of the exception handling system. In
 * a nutshell, function libraries can use @c #try, @c #catch, @c #throw, etc.
 * whether the client previously began an exception context or not.
 *
 * You <strong>must not</strong> use this macro unless you are implementing some
 * functionality which is to be called from another program, potentially unaware
 * of exceptions.
 *
 * When the block completes, the system returns to its previous status (if it
 * was required to open a new exception context, then it will be automatically
 * closed).
 *
 * This way, when an external functions encounters an error, it may either throw
 * an exception (when the caller is aware of the exception system), or otherwise
 * return an error code (when the caller did not open an exception context).
 *
 * @c #e4c_reusing_context needs to be given a @c status variable (or @e lvalue)
 * that will be assigned a specified failure value when an exception is thrown
 * inside the block. The failure @e rvalue can be any expression assignable to
 * the specified status @e lvalue.
 *
 * @code
 * int library_public_function(void * pointer, int number){
 *
 *     // We don't know where this function is going to be called from, so:
 *     //   * We cannot use "try", "throw", etc. right here, because the
 *     //   exception context COULD be uninitialized at this very moment.
 *     //   * We cannot call "e4c_context_begin" either, because the
 *     //   exception context COULD be already initialized.
 *     // If we have to make use of the exception handling system, we need to
 *     // "reuse" the existing exception context or "use" a new one.
 *
 *     volatile int status = STATUS_OK;
 *
 *     e4c_reusing_context(status, STATUS_ERROR){
 *
 *         // Now we can safely use "try", "throw", etc.
 *         if(pointer == NULL){
 *             throw(NullPointerException);
 *         }
 *
 *         library_private_function(pointer, number);
 *     }
 *
 *     return(status);
 * }
 * @endcode
 *
 * The status will be left unmodified if the client (i.e. the function caller)
 * is @e exception-aware, or the block @e completes without an error (i.e. no
 * exception is thrown), so it @b must be properly initialized before returning
 * it.
 *
 * Please note that the status needs not be just a dichotomy (success or
 * failure). It can be a fine-grained value describing what exactly went wrong.
 * You can pass any expression to @c #e4c_reusing_context wich will be evaluated
 * if an exception is thrown:
 *
 * @code
 * int library_public_function(void * pointer, int number){
 *
 *     volatile int status = STATUS_OK;
 *     volatile bool flag = true;
 *
 *     e4c_reusing_context(status, (flag ? STATUS_NULL_POINTER : STATUS_ERROR)){
 *
 *         if(pointer == NULL){
 *             throw(NullPointerException);
 *         }
 *
 *         flag = false;
 *
 *         library_private_function(pointer, number);
 *     }
 *
 *     return(status);
 * }
 * @endcode
 *
 * However, Most of the times you probably want to yield a different status
 * value depending on the specific exception being thrown. This can be easily
 * accomplished by making use of the macro @c #E4C_ON_FAILURE.
 *
 * Next, the semantics of @c #e4c_reusing_context are explained, step by step:
 *
 * <ul>
 *   <li>
 *   If there is an exception context at the time the block starts:
 *   <ol>
 *     <li>The existing exception context will be reused.</li>
 *     <li>The code block will take place.</li>
 *     <li>
 *       If any exception is thrown during the execution of the block:
 *       <ul>
 *       <li>It will be @b propagated upwards to the caller.</li>
 *       <li>The control will be transferred to the nearest surrounding block of
 *         code which is able to handle that exception.</li>
 *       </ul>
 *     </li>
 *   </ol>
 *   </li>
 *   <li>
 *   If there is no exception context at the time the block starts:
 *   <ol>
 *     <li>A new exception context will be begun; note that the signal handling
 *       system <strong>WILL NOT</strong> be set up.</li>
 *     <li>The code block will take place.</li>
 *     <li>
 *       If any exception is thrown during the execution of the block:
 *       <ul>
 *           <li>It will be @b caught.</li>
 *           <li>@c status will be asigned the value of the expression
 *             @c on_failure.</li>
 *       </ul>
 *     </li>
 *   </ol>
 *   </li>
 * </ul>
 *
 * If you need to perform any cleanup, you should place it @e inside a
 * @c #finally block, for example:
 *
 * @code
 * ...
 * e4c_reusing_context(status, STATUS_ERROR){
 *
 *     void * buffer = NULL;
 *     try{
 *         buffer = malloc(1024);
 *         ...
 *     }finally{
 *         free(buffer);
 *     }
 * }
 * ...
 * @endcode
 *
 * If you need to rely on the signal handling system, you may call
 * @c #e4c_context_set_signal_mappings explicitly. You should take into account
 * that you could be @e hijacking your client's original signal mappings, so you
 * should also call @c #e4c_context_get_signal_mappings in order to restore the
 * previous signal mappings when you are done.
 *
 * @code
 * const e4c_signal_mapping new_mappings[] = {
 *     E4C_SIGNAL_MAPPING(SIGABRT, Exception1),
 *     E4C_SIGNAL_MAPPING(SIGINT, Exception2),
 *     E4C_IGNORE_SIGNAL(SIGTERM),
 *     ...
 *     E4C_NULL_SIGNAL_MAPPING
 * };
 * ...
 * e4c_reusing_context(status, STATUS_ERROR){
 *
 *     const e4c_signal_mapping * old_mappings = e4c_context_get_signal_mappings();
 *
 *     e4c_context_set_signal_mappings(new_mappings);
 *
 *     try{
 *         ...
 *     }finally{
 *         e4c_context_set_signal_mappings(old_mappings);
 *     }
 * }
 * ...
 * @endcode
 *
 * This macro only begins a new exception context <strong>if there is no one,
 * already begun, to be used</strong> whereas @c #e4c_using_context always
 * attempts to begin a new one.
 *
 * @pre     A block introduced by @c #e4c_reusing_context <strong>must
 *          not</strong> be exited through any of: @c goto, @c break,
 *          @c continue or @c return (but it is legal to @c #throw an
 *          exception).
 * @post    A block introduced by @c #e4c_reusing_context is guaranteed to take
 *          place @e inside an exception context.
 *
 * @see     #e4c_context_begin
 * @see     #e4c_context_end
 * @see     #e4c_context_is_ready
 * @see     #e4c_using_context
 * @see     #e4c_exception
 * @see     #E4C_ON_FAILURE
 */
# define e4c_reusing_context(_status_, _on_failure_) \
	E4C_REUSING_CONTEXT(_status_, _on_failure_)

/**
 * Provides a means of parsing an exception to obtain a status value
 *
 * @param   _handler_
 *          The name of the parser function to be called
 *
 * This is a handy way to call a function when a #e4c_reusing_context block
 * fails. This function will be passed the current thrown exception; it is
 * expected to parse it and return a proper status value.
 *
 * @code
 * static int parse_exception(const e4c_exception * exception){
 *
 *     if(exception->type == &NotEnoughMemoryException){
 *         return(STATUS_MEMORY_ERROR);
 *     }else if( is_instance_of(exception, &MyException) ){
 *         return(STATUS_MY_ERROR);
 *     }
 *
 *     return(STATUS_ERROR);
 * }
 *
 * int library_public_function(void * pointer, int number){
 *
 *     volatile int status = STATUS_OK;
 *
 *     e4c_reusing_context(status, E4C_ON_FAILURE(parse_exception)){
 *
 *         if(pointer == NULL){
 *             throw(NullPointerException);
 *         }
 *
 *         library_private_function(pointer, number);
 *     }
 *
 *     return(status);
 * }
 * @endcode
 *
 * @see     #e4c_reusing_context
 * @see     #e4c_get_exception
 * @see     #e4c_exception
 */
# define E4C_ON_FAILURE(_handler_) _handler_( e4c_get_exception() )

/**
 * Marks a function which never returns
 *
 * This macro helps both developer and compiler to assume that the marked
 * function will not return the control to its caller (unless by throwing an
 * exception).
 *
 * @note
 * It does not make sense for these functions to have a return type other than
 * @c void.
 *
 * For example, a function @c f1 that @b always throws an exception, could be
 * marked with this macro:
 *
 * @code
 * void f1(int foo) E4C_NO_RETURN;
 * // ...
 * void f1(int foo){
 *     if(foo == 1){
 *         throw(MyException1, "foo is one.");
 *     }
 *     throw(MyException2, "foo is not one.");
 * }
 * @endcode
 *
 * Then, if another function tested a condition and then called @c f1, it
 * wouldn't need to return anything witnin the @c if branch, nor consider the
 * @c else branch of the test:
 *
 * @code
 * int f2(int bar, int foo){
 *
 *     if(bar == 0){
 *         f1(foo);
 *         // return(-1);
 *     }// else
 *
 *     return(123);
 *
 * }
 * @endcode
 *
 * If the compiler supports this macro, it could optimize the program and avoid
 * spurious warnings of uninitialized variables.
 *
 * @see     #E4C_UNREACHABLE_RETURN
 */
# define E4C_NO_RETURN \
	\
	E4C_NO_RETURN_

/**
 * Simulates a function return
 *
 * @param   _value_
 *          The value that would be returned if the statement took place.
 *
 * This macro ensures portability on compilers which don't support functions
 * that never return.
 *
 * It may be used after calling a function marked as @c #E4C_NO_RETURN, so that
 * the compiler will not complain about <em>control reaching end of non-void
 * function</em>. For example:
 *
 * @code
 * void f1(int foo) E4C_NO_RETURN;
 *
 * int f3(int bar, int foo){
 *
 *     if(bar != 0){
 *         return(123);
 *     }
 *
 *     f1(123);
 *
 *     E4C_UNREACHABLE_RETURN(-1);
 * }
 * @endcode
 *
 * This macro will become an actual @c return statement if the compiler does not
 * support @c #E4C_NO_RETURN, even though it will never be reached (because the
 * called function won't actually return control).
 *
 * @see     #E4C_NO_RETURN
 * @see     #E4C_UNREACHABLE_VOID_RETURN
 */
# define E4C_UNREACHABLE_RETURN(_value_) \
	\
	E4C_UNREACHABLE_RETURN_(_value_)

/**
 * Simulates a function void return
 *
 * This macro ensures portability on static source code analyzers which don't
 * support functions that never return.
 *
 * It may be used after calling a function marked as @c #E4C_NO_RETURN, so that
 * the analyzer will not complain about spurious errors. For example, if we
 * didn't use <tt>E4C_UNREACHABLE_VOID_RETURN</tt> here, some analyzers might
 * complain about <em>possible null pointer dereference</em> at line
 * <tt>foo = *bar</tt>, because they are not aware that function call
 * <tt>f1(321);</tt> will never return control:
 *
 * @code
 * void f1(int foo) E4C_NO_RETURN;
 *
 * void f3(int * bar){
 *
 *     int foo;
 *
 *     if(bar == NULL){
 *         f1(321);
 *         E4C_UNREACHABLE_VOID_RETURN;
 *     }
 *
 *     foo = *bar;
 *     printf("value: %d", foo);
 * }
 * @endcode
 *
 * This macro will become an actual @c return statement if the compiler does not
 * support @c #E4C_NO_RETURN, even though it will never be reached (because the
 * called function won't actually return control).
 *
 * @see     #E4C_NO_RETURN
 * @see     #E4C_UNREACHABLE_RETURN
 */
# define E4C_UNREACHABLE_VOID_RETURN \
	\
	E4C_UNREACHABLE_VOID_RETURN_

/** @} */

/**
 * @name Other convenience macros
 *
 * These macros provide a handy way to: begin (and end) implicitly a new
 * exception context, express @e assertions, define and declare exceptions, and
 * define arrays of signal mappings.
 *
 * @{
 */

/**
 * Introduces a block of code which will use a new exception context.
 *
 * @param   _handle_signals_
 *          If @c true, the signal handling system will be set up with the
 *          default mapping.
 *
 * This macro begins a new exception context to be used by the code block right
 * next to it. When the code completes, @c #e4c_context_end will be called
 * implicitly.
 *
 * This macro is very convenient when the beginning and the ending of the
 * current exception context are next to each other. For example, there is no
 * semantic difference between this two blocks of code:
 *
 * @code
 *
 * // block 1
 * e4c_context_begin(E4C_TRUE);
 * // ...
 * e4c_context_end();
 *
 * // block 2
 * e4c_using_context(E4C_TRUE){
 *     // ...
 * }
 * @endcode
 *
 * This macro @b always attempts to begin a new exception context, whereas
 * @c #e4c_reusing_context only does if there is no exception context, already
 * begun, to be used.
 *
 * This macro <strong>should be used whenever possible</strong>, rather than
 * doing the explicit, manual calls to @c #e4c_context_begin and
 * @c #e4c_context_end, because it is less prone to errors.
 *
 * @pre     A block introduced by @c #e4c_using_context <strong>must
 *          not</strong> be exited through any of: @c goto, @c break,
 *          @c continue or @c return (but it is legal to @c #throw an
 *          exception).
 * @post    A block introduced by @c #e4c_using_context is guaranteed to take
 *          place @e inside an exception context.
 *
 * @see     #e4c_context_begin
 * @see     #e4c_context_end
 * @see     #e4c_reusing_context
 */
# define e4c_using_context(_handle_signals_) \
	E4C_USING_CONTEXT(_handle_signals_)

/**
 * Expresses a program assertion
 *
 * @param   _condition_
 *          A predicate that must evaluate to @c true
 *
 * An assertion is a mechanism to express that the developer @e thinks that a
 * specific condition is always met at some point of the program.
 *
 * @c #assert is a convenient way to insert debugging assertions into a
 * program. The @c NDEBUG @e compile-time parameter determines whether the
 * assumptions will be actually verified by the program at @e run-time.
 *
 * In presence of @c NDEBUG, the assertion statements will be ignored and
 * therefore will have no effect on the program, not even evaluating the
 * condition. Therefore expressions passed to @c #assert <strong>must not
 * contain @e side-effects</strong>, since they will not take place when
 * debugging is disabled.
 *
 * In absence of @c NDEBUG, the assertion statements will verify that the
 * condition is met every time the program reaches that point of the program.
 *
 * If the assertion does not hold at any time, then an @c #AssertionException
 * will be thrown to indicate the programming error. This exception cannot be
 * caught whatsoever. The program (or current thread) will be terminated.
 *
 * The main advantage of using this assertion mechanism (as opposed to the
 * macros provided by the standard header file <code>assert.h</code>) is that
 * before actually exiting the program or thread, all of the pending @c #finally
 * blocks will be executed.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #assert. Such programming error will lead to an
 *          an abrupt exit of the program (or thread).
 *
 * @see     #AssertionException
 */
# ifndef E4C_NOKEYWORDS
# ifdef assert
	/* macro assert is already defined (probably assert.h was included) */
#	error "Please define E4C_NOKEYWORDS at compiler level " \
"in order to prevent exceptions4c from defining the assert macro."
# endif
/*@notfunction@*/
#	define assert(_condition_) \
		E4C_ASSERT(_condition_)
# endif

/**
 * Throws an exception with a formatted message
 *
 * @param   _exception_type_
 *          The type of exception to be thrown
 * @param   _format_
 *          The string containing the specifications that determine the output
 *          format for the variadic arguments
 * @param   ...
 *          The variadic arguments that will be formatted according to the
 *          format control
 *
 * This is a handy way to compose a formatted message and throw an exception
 * <em>on the fly</em>:
 *
 * @code
 * int bytes = 1024;
 * void * buffer = malloc(bytes);
 * if(buffer == NULL){
 *     throwf(NotEnoughMemoryException, "Could not allocate %d bytes.", bytes);
 * }
 * @endcode
 *
 * This macro relies on two features that were introduced in the
 * <strong>ISO/IEC 9899:1999</strong> (also known as @e C99) revision of the C
 * programming language standard in 1999:
 *
 * @li Variadic macros
 * @li Buffer-safe function @c vsnprintf
 *
 * In order not to create compatibility issues, this macro will only be defined
 * when the @c __STDC_VERSION__ @e compile-time parameter is defined as a
 * @c long value <em>greater than or equal to</em> @c 199901L, or when
 * @c HAVE_C99_VARIADIC_MACROS is defined.
 *
 * The semantics of this keyword are the same as for @c #throw.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #throwf. Such programming error will lead to an
 *          abrupt exit of the program (or thread).
 * @pre		At least one argument @b must be passed right after the format
 *          string. The message will be composed through the function
 *          @c vsnprintf with the specified format and variadic arguments. For
 *          further information on formatting rules, you may look up the
 *          specifications for the function @c vsnprintf.
 * @post    Control does not return to the @c #throwf point.
 *
 * @see     #throw
 * @see     #rethrowf
 */
# if !defined(E4C_NOKEYWORDS) && defined(HAVE_C99_VARIADIC_MACROS)
#	define throwf(_exception_type_, _format_, ...) \
		\
		E4C_THROWF( (_exception_type_), (_format_), __VA_ARGS__ )
# endif

/**
 * Throws again the currently thrown exception, with a new, formatted message
 *
 * @param   _format_
 *          The string containing the specifications that determine the output
 *          format for the variadic arguments.
 * @param   ...
 *          The variadic arguments that will be formatted according to the
 *          format control.
 *
 * This is a handy way to create (and then @c #throw) a new instance of the
 * currently thrown exception, with a more specific, formatted message.
 *
 * @code
 * try{
 *     image = read_file(file_path);
 * }catch(FileOpenException){
 *     rethrowf("Image '%s' not found.", title);
 * }
 * @endcode
 *
 * This macro relies on two features that were introduced in the
 * <strong>ISO/IEC 9899:1999</strong> (also known as @e C99) revision of the C
 * programming language standard in 1999:
 *
 * @li Variadic macros
 * @li Buffer-safe function @c vsnprintf
 *
 * In order not to create compatibility issues, this macro will only be defined
 * when the @c __STDC_VERSION__ @e compile-time parameter is defined as a
 * @c long value <em>greater than or equal to</em> @c 199901L, or when
 * @c HAVE_C99_VARIADIC_MACROS is defined.
 *
 * The semantics of this keyword are the same as for @c #throw.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          using the keyword @c #rethrowf. Such programming error will lead to
 *          an abrupt exit of the program (or thread).
 * @pre		At least one argument @b must be passed right after the format
 *          string. The message will be composed through the function
 *          @c vsnprintf with the specified format and variadic arguments. For
 *          further information on formatting rules, you may look up the
 *          specifications for the function @c vsnprintf.
 * @post    Control does not return to the @c #rethrowf point.
 *
 * @see     #rethrow
 * @see     #throwf
 */
# if !defined(E4C_NOKEYWORDS) && defined(HAVE_C99_VARIADIC_MACROS)
#	define rethrowf(_format_, ...) \
		\
		E4C_RETHROWF( (_format_), __VA_ARGS__ )
# endif


/**
 * Declares an exception type
 *
 * @param   _name_
 *          Name of the exception type
 *
 * This macro introduces the name of an @c extern, @c const exception type which
 * will be available to be thrown or caught:
 *
 * @code
 * E4C_DECLARE_EXCEPTION(StackException);
 * E4C_DECLARE_EXCEPTION(StackOverflowException);
 * E4C_DECLARE_EXCEPTION(StackUnderflowException);
 * @endcode
 *
 * This macro is intended to be used inside header files.
 *
 * @note
 * When you @e declare exception types, no storage is allocated. In order to
 * actually @e define them, you need to use the macro @c #E4C_DEFINE_EXCEPTION.
 *
 * @see     #e4c_exception_type
 * @see     #E4C_DEFINE_EXCEPTION
 */
# define E4C_DECLARE_EXCEPTION(_name_) \
	\
	extern const e4c_exception_type _name_

/**
 * Defines an exception type
 *
 * @param   _name_
 *          Name of the new exception type
 * @param   _default_message_
 *          Default message of the new exception type
 * @param   _supertype_
 *          Supertype of the new exception type
 *
 * This macro allocates a new, @c const exception type.
 *
 * @code
 * E4C_DEFINE_EXCEPTION(StackException, "Stack exception", RuntimeException);
 * E4C_DEFINE_EXCEPTION(StackOverflowException, "Stack overflow", StackException);
 * E4C_DEFINE_EXCEPTION(StackUnderflowException, "Stack underflow", StackException);
 * @endcode
 *
 * This macro is intended to be used inside sorce code files. The defined
 * exception types can be @e declared in a header file through the macro
 * @c #E4C_DECLARE_EXCEPTION.
 *
 * @see     #e4c_exception_type
 * @see     #RuntimeException
 * @see     #E4C_DECLARE_EXCEPTION
 */
# define E4C_DEFINE_EXCEPTION(_name_, _default_message_, _supertype_) \
	\
	const e4c_exception_type _name_ = { \
		#_name_, \
		_default_message_, \
		&_supertype_ \
	}

/**
 * Maps a specific signal number to a given exception type
 *
 * @param   _signal_number_
 *          Numeric value of the signal to be converted
 * @param   _exception_type_
 *          Exception type representing the signal
 *
 * This macro represents a signal mapping literal. It comes in handy for
 * initializing arrays of @c #e4c_signal_mapping.
 *
 * @see     #e4c_signal_mapping
 * @see     #e4c_context_set_signal_mappings
 * @see     #e4c_context_get_signal_mappings
 * @see     #E4C_IGNORE_SIGNAL
 * @see     #E4C_NULL_SIGNAL_MAPPING
 * @see     #E4C_DECLARE_EXCEPTION
 */
# define E4C_SIGNAL_MAPPING(_signal_number_, _exception_type_) \
	\
	{_signal_number_, &_exception_type_}

/**
 * Ignores a specific signal number
 *
 * @param   _signal_number_
 *          Numeric value of the signal to be ignored
 *
 * This macro represents a signal mapping literal. It comes in handy for
 * initializing arrays of @c #e4c_signal_mapping.
 *
 * @see     #e4c_signal_mapping
 * @see     #e4c_context_set_signal_mappings
 * @see     #e4c_context_get_signal_mappings
 * @see     #E4C_SIGNAL_MAPPING
 * @see     #E4C_NULL_SIGNAL_MAPPING
 * @see     #E4C_DECLARE_EXCEPTION
 */
# define E4C_IGNORE_SIGNAL(_signal_number_) \
	\
	{_signal_number_, NULL}

/**
 * Represents a null signal mapping literal
 *
 * This macro comes in handy for terminating arrays of @c #e4c_signal_mapping.
 *
 * @see     #e4c_signal_mapping
 * @see     #e4c_context_set_signal_mappings
 * @see     #e4c_context_get_signal_mappings
 * @see     #E4C_SIGNAL_MAPPING
 * @see     #E4C_IGNORE_SIGNAL
 * @see     #E4C_DECLARE_EXCEPTION
 */
# define E4C_NULL_SIGNAL_MAPPING \
	\
	{E4C_INVALID_SIGNAL_NUMBER_, NULL}

/** @} */


/**
 * Represents an exception type in the exception handling system
 *
 * The types of the exceptions a program will use are @b defined in source code
 * files through the macro @c #E4C_DEFINE_EXCEPTION. In addition, they are
 * @b declared in header files through the macro @c #E4C_DECLARE_EXCEPTION.
 *
 * When defining types of exceptions, they are given a @e name, a <em>default
 * message</em> and a @e supertype to organize them into a @e pseudo-hierarchy:
 *
 * @code
 * E4C_DEFINE_EXCEPTION(SimpleException, "Simple exception", RuntimeException);
 * @endcode
 *
 * Exceptions are usually defined as global objects. There is a set of
 * predefined exceptions built into the framework, and @c #RuntimeException is
 * the root of the exceptions @e pseudo-hierarchy:
 *
 * <ul>
 *     <li>@c #RuntimeException</li>
 *     <ul>
 *         <li>@c #NotEnoughMemoryException</li>
 *         <li>@c #AssertionException</li>
 *         <li>@c #IllegalArgumentException</li>
 *         <li>@c #InputOutputException</li>
 *         <li>@c #SignalException</li>
 *         <ul>
 *             <li>@c #SignalAlarmException</li>
 *             <li>@c #SignalChildException</li>
 *             <li>@c #SignalTrapException</li>
 *             <li>@c #ErrorSignalException</li>
 *             <ul>
 *                 <li>@c #IllegalInstructionException</li>
 *                 <li>@c #ArithmeticException</li>
 *                 <li>@c #BrokenPipeException</li>
 *                 <li>@c #BadPointerException</li>
 *                 <ul>
 *                     <li>@c #NullPointerException</li>
 *                 </ul>
 *             </ul>
 *             <li>@c #ControlSignalException</li>
 *             <ul>
 *                 <li>@c #StopException</li>
 *                 <li>@c #KillException</li>
 *                 <li>@c #HangUpException</li>
 *                 <li>@c #TerminationException</li>
 *                 <li>@c #AbortException</li>
 *                 <li>@c #CPUTimeException</li>
 *                 <li>@c #UserControlSignalException</li>
 *                 <ul>
 *                     <li>@c #UserQuitException</li>
 *                     <li>@c #UserInterruptionException</li>
 *                     <li>@c #UserBreakException</li>
 *                 </ul>
 *             </ul>
 *             <li>@c #ProgramSignalException</li>
 *             <ul>
 *                 <li>@c #ProgramSignal1Exception</li>
 *                 <li>@c #ProgramSignal2Exception</li>
 *             </ul>
 *         </ul>
 *     </ul>
 * </ul>
 *
 * @see     #e4c_exception
 * @see     #E4C_DEFINE_EXCEPTION
 * @see     #E4C_DECLARE_EXCEPTION
 * @see     #throw
 * @see     #catch
 */
typedef struct e4c_exception_type_ e4c_exception_type;
struct e4c_exception_type_{

	/** The name of this exception type */
	/*@observer@*/ /*@notnull@*/
	const char *					name;

	/** The default message of this exception type */
	/*@observer@*/
	const char						default_message[E4C_EXCEPTION_MESSAGE_SIZE];

	/** The supertype of this exception type */
	/*@shared@*/ /*@notnull@*/
	const e4c_exception_type *		supertype;
};

/**
 * Represents an instance of an exception type
 *
 * Exceptions are a means of breaking out of the normal flow of control of a
 * code block in order to handle errors or other exceptional conditions. An
 * exception should be thrown at the point where the error is detected; it may
 * be handled by the surrounding code block or by any code block that directly
 * or indirectly invoked the code block where the error occurred.
 *
 * Exceptions provide information regarding the exceptional situation, such as:
 *
 * @li The exception @c name
 * @li An @e ad-hoc @c message (as opposed to the @e default one)
 * @li The exact point of the program where it was thrown (source code @c file,
 *          @c line and @c function name, if available)
 * @li The value of the standard error code @c errno at the time the exception
 *          was thrown
 * @li The @c cause of the exception, which is the previous exception (if any),
 *          when the exception was thrown
 * @li The specific @c type of the exception, convenient when handling an
 *          abstract type of exceptions from a @c #catch block
 * @li Optional, @e user-defined, @c custom_data, which can be initialized and
 *          finalized throught context @e handlers
 *
 * @note
 * @b Any exception can be caught by a block introduced by
 * @c #catch( @c #RuntimeException ), <strong>except for
 * @c #AssertionException</strong>.
 *
 * @see     #e4c_exception_type
 * @see     #throw
 * @see     #catch
 * @see     #e4c_get_exception
 * @see     #e4c_context_set_handlers
 * @see     #RuntimeException
 * @see     #AssertionException
 */
typedef struct e4c_exception_ e4c_exception;
struct e4c_exception_{

	/* This field is undocumented on purpose and reserved for internal use */
	int								_;

	/** The name of this exception */
	/*@observer@*/ /*@notnull@*/
	const char *					name;

	/** The message of this exception */
	/*@exposed@*/
	char							message[E4C_EXCEPTION_MESSAGE_SIZE];

	/** The path of the source code file from which the exception was thrown */
	/*@observer@*/ /*@null@*/
	const char *					file;

	/** The number of line from which the exception was thrown */
	int								line;

	/** The function from which the exception was thrown */
	/*@observer@*/ /*@null@*/
	const char *					function;

	/** The value of errno at the time the exception was thrown */
	int								error_number;

	/** The type of this exception */
	/*@shared@*/ /*@notnull@*/
	const e4c_exception_type *		type;

	/** The cause of this exception */
	/*@only@*/ /*@null@*/
	e4c_exception *					cause;

	/** Custom data associated to this exception */
	/*@shared@*/ /*@null@*/
	void *							custom_data;
};

/**
 * Represents a map between a signal and an exception
 *
 * A signal is an asynchronous notification sent by the operating system to a
 * process in order to notify it of an event that occurred. Most of the signals
 * will, by default, crash the program as soon as they are raised.
 * @b exceptions4c can convert signals to exceptions, so they can be easily
 * handled.
 *
 * For example, a @e suspicious or @e dangerous part of the program could be
 * wrapped up with @c #try blocks and then @c #catch segmentation faults or
 * divisions by zero. Then the program would clean up and continue normally:
 *
 * @code
 * e4c_using_context(E4C_TRUE){
 *     int * pointer = NULL;
 *     try{
 *         int oops = *pointer;
 *     }catch(BadPointerException){
 *         printf("No problem ;-)");
 *     }finally{
 *         // clean up...
 *     }
 * }
 * @endcode
 *
 * In order to perform the conversion, @b exceptions4c @e maps signals to
 * exceptions.
 *
 * The simplest way to get this working is by calling the function
 * @c #e4c_context_begin. This function will set up the default mappings for the
 * available signals in the platform, when passed @c handle_signals=true.
 *
 * If you need to be more specific about which signals get converted to
 * exceptions, you can define an array of signal mappings and then pass it to
 * the function @c #e4c_context_set_signal_mappings.
 *
 * An array of signal mappings is defined through the macros
 * @c #E4C_SIGNAL_MAPPING, @c #E4C_IGNORE_SIGNAL and
 * @c #E4C_NULL_SIGNAL_MAPPING.
 *
 * While @c #E4C_SIGNAL_MAPPING tells the system to convert a specific signal to
 * a given exception, @c #E4C_IGNORE_SIGNAL allows you to disregard the signal
 * and continue (even if unmeaningful).
 *
 * Every @c #e4c_signal_mapping array needs to be terminated with the
 * @c #E4C_NULL_SIGNAL_MAPPING element, so the system finds out how many
 * mappings are there in a given array.
 *
 * @code
 * const e4c_signal_mapping my_signal_mappings[] = {
 *     E4C_SIGNAL_MAPPING(SIGABRT, Exception1),
 *     E4C_SIGNAL_MAPPING(SIGINT, Exception2),
 *     E4C_IGNORE_SIGNAL(SIGTERM),
 *     ...
 *     E4C_NULL_SIGNAL_MAPPING
 * }
 * @endcode
 *
 * Once the array is properly defined, it can be passed to the function
 * @c #e4c_context_set_signal_mappings. This way, only the specified signals
 * will be handled as exceptions, and they will be converted to the specified
 * exceptions.
 *
 * @code
 * e4c_using_context(E4C_FALSE){
 *
 *     e4c_context_set_signal_mappings(my_signal_mappings);
 *     ...
 * }
 * @endcode
 *
 * These are some of the signals you can handle:
 *
 * @li @c SIGFPE when dividing by zero.
 * @li @c SIGSEGV when dereferencing an invalid pointer.
 * @li @c SIGINT when a user interrupts the process.
 * @li @c SIGTERM when a process is requested to be terminated.
 *
 * @see     #e4c_context_begin
 * @see     #e4c_context_set_signal_mappings
 * @see     #e4c_context_get_signal_mappings
 * @see     #E4C_SIGNAL_MAPPING
 * @see     #E4C_IGNORE_SIGNAL
 * @see     #e4c_default_signal_mappings
 */
typedef struct e4c_signal_mapping_ e4c_signal_mapping;
struct e4c_signal_mapping_{

	/** The signal to be converted */
	int									signal_number;

	/** The exception representing the signal */
	/*@dependent@*/ /*@null@*/
	const e4c_exception_type * const	exception_type;

};

/**
 * Represents the completeness of a code block aware of exceptions
 *
 * The symbolic values representing the status of a block help to distinguish
 * between different possible situations inside a @c #finally block. For
 * example, different cleanup actions can be taken, depending on the status of
 * the block.
 *
 * @code
 * try{
 *    ...
 * }finally{
 *    switch( e4c_get_status() ){
 *
 *        case e4c_succeeded:
 *            ...
 *            break;
 *
 *        case e4c_recovered:
 *            ...
 *            break;
 *
 *        case e4c_failed:
 *            ...
 *            break;
 *    }
 * }
 * @endcode
 *
 * @see     #e4c_get_status
 * @see     #finally
 */
enum e4c_status_{

	/** There were no exceptions */
	e4c_succeeded,

	/** There was an exception, but it was caught */
	e4c_recovered,

	/** There was an exception and it wasn't caught */
	e4c_failed
};
typedef enum e4c_status_ e4c_status;

/**
 * Represents a function which will be executed in the event of an uncaught
 * exception.
 *
 * @param   exception
 *          The uncaught exception
 *
 * This handler can be set through the function @c #e4c_context_set_handlers:
 *
 * @code
 * void my_uncaught_handler(const e4c_exception * exception){
 *
 *     printf("Error: %s (%s)\n", exception->name, exception->message);
 * }
 *
 * int main(int argc, char * argv[]){
 *
 *     e4c_using_context(E4C_TRUE){
 *
 *         e4c_context_set_handlers(my_uncaught_handler, NULL, NULL, NULL);
 *         // ...
 *     }
 * }
 * @endcode
 *
 * There exists a convenience function @c #e4c_print_exception which is used as
 * the default <em>uncaught handler</em>, unless otherwise specified. It simply
 * prints information regarding the exception to @c stderr.
 *
 * @warning
 * These functions are not allowed to try and recover the current exception
 * context. Moreover, the program (or current thread) will terminate right after
 * the function returns.
 *
 * @see     #e4c_context_set_handlers
 * @see     #e4c_initialize_handler
 * @see     #e4c_finalize_handler
 * @see     #e4c_print_exception
 */
typedef void (*e4c_uncaught_handler)(const e4c_exception * exception)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
*/
;

/**
 * Represents a function which will be executed whenever a new exception is
 * thrown.
 *
 * @param   exception
 *          The newly thrown exception
 *
 * When this handler is set, it will be called any time a new exception is
 * created. The @c void pointer returned by this function will be assigned to
 * the exception's @e custom_data. This data can be accessed later on, for
 * example, from a @c catch block, or an <em>uncaught handler</em>, for any
 * specific purpose.
 *
 * This handler can be set through the function @c #e4c_context_set_handlers:
 *
 * @code
 * void * my_initialize_handler(const e4c_exception * e){
 *
 *     if( e4c_is_instance_of(e, &SignalException) ){
 *         printf("Program received signal %s (%d)!\n", e->file, e->line);
 *     }
 *
 *     return(NULL);
 * }
 *
 * int main(int argc, char * argv[]){
 *
 *     e4c_using_context(E4C_TRUE){
 *
 *         e4c_context_set_handlers(NULL, NULL, my_initialize_handler, NULL);
 *         // ...
 *     }
 * }
 * @endcode
 *
 * By the time this handler is called, the exception already has been assigned
 * the initial value specified fot @c custom_data, so the handler may make use
 * of it:
 *
 * @code
 * void * log_handler(const e4c_exception * e){
 *
 *     printf("LOG: Exception thrown at module '%s'\n", e->custom_data);
 *
 *     return(NULL);
 * }
 *
 * int main(int argc, char * argv[]){
 *
 *     e4c_using_context(E4C_TRUE){
 *
 *         e4c_context_set_handlers(NULL, "FOO", log_handler, NULL);
 *         // ...
 *     }
 *
 *     e4c_using_context(E4C_TRUE){
 *
 *         e4c_context_set_handlers(NULL, "BAR", log_handler, NULL);
 *         // ...
 *     }
 * }
 * @endcode
 *
 * @see     #e4c_context_set_handlers
 * @see     #e4c_exception
 * @see     #e4c_finalize_handler
 * @see     #e4c_uncaught_handler
 * @see     #e4c_print_exception
 */
typedef void * (*e4c_initialize_handler)(const e4c_exception * exception)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
*/
;

/**
 * Represents a function which will be executed whenever an exception is
 * destroyed.
 *
 * @param   custom_data
 *          The "custom data" of the exception to be discarded
 *
 * When this handler is set, it will be called any time an exception is
 * discarded. It will be passed the <em>custom_data</em> of the exception, so
 * it can dispose resources previously acquired by the
 * <em>initialize handler</em>.
 *
 * This handler can be set through the function @c #e4c_context_set_handlers:
 *
 * @code
 * void * initialize_data(const e4c_exception * exception){
 *
 *     const char * custom_data = malloc(1024);
 *
 *     if(custom_data != NULL){
 *         if( e4c_is_instance_of(exception, &SignalException) ){
 *             strcpy(custom_data, "SIGNAL ERROR");
 *         }else{
 *             strcpy(custom_data, "RUNTIME ERROR");
 *         }
 *     }
 *
 *     return(custom_data);
 * }
 *
 * void * finalize_data(void * custom_data){
 *
 *     free(custom_data);
 * }
 *
 * int main(int argc, char * argv[]){
 *
 *     e4c_using_context(E4C_TRUE){
 *
 *         e4c_context_set_handlers(NULL, NULL, initialize_data, finalize_data);
 *         ...
 *     }
 * }
 * @endcode
 *
 * @see     #e4c_context_set_handlers
 * @see     #e4c_exception
 * @see     #e4c_finalize_handler
 * @see     #e4c_uncaught_handler
 * @see     #e4c_print_exception
 */
typedef void (*e4c_finalize_handler)(void * custom_data)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
*/
;

/*
 * Next types are undocumented on purpose, in order to hide implementation
 * details, subject to change.
 */
enum e4c_frame_stage_{
	e4c_beginning_,
	e4c_acquiring_,
	e4c_trying_,
	e4c_disposing_,
	e4c_catching_,
	e4c_finalizing_,
	e4c_done_
};

struct e4c_continuation_{
	/*@partial@*/ /*@dependent@*/
	E4C_CONTINUATION_BUFFER_		buffer;
};

/**
 * @name Predefined signal mappings
 *
 * There is a predefined set of signal mappings. Signal mappings are used to
 * convert signals into exceptions.
 *
 * Common signals are mapped to its corresponding exception, for example:
 *
 * @li @c SIGABRT is mapped to @c #AbortException
 * @li @c SIGFPE is mapped to @c #ArithmeticException
 * @li @c SIGSEGV is mapped to @c #BadPointerException
 * @li @c SIGTERM is mapped to @c #TerminationException
 * @li ...and so on
 *
 * @see     #e4c_signal_mapping
 * @see     #e4c_context_begin
 * @see     #e4c_context_set_signal_mappings
 * @see     #e4c_context_get_signal_mappings
 * @{
 */

/**
 * The array of predefined signal mappings.
 */
/*@unused@*/ /*@notnull@*/
extern const e4c_signal_mapping * const e4c_default_signal_mappings;

/** @} */

/**
 * @name Predefined exceptions
 *
 * Built-in exceptions represent usual error conditions that might occur during
 * the course of any program.
 *
 * They are organized into a @e pseudo-hierarchy, being @c #RuntimeException
 * the common @e supertype of all exceptions.
 *
 * @{
 */

/**
 * This is the root of the exception pseudo-hierarchy
 *
 * @c #RuntimeException is the common @e supertype of all exceptions.
 *
 * @par     Direct known subexceptions:
 *          #NotEnoughMemoryException,
 *          #AssertionException,
 *          #IllegalArgumentException,
 *          #InputOutputException,
 *          #SignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(RuntimeException);

/**
 * This exception is thrown when the system runs out of memory
 *
 * @c #NotEnoughMemoryException is thrown when there is not enough memory to
 * continue the execution of the program.
 *
 * @par     Extends:
 *          #RuntimeException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(NotEnoughMemoryException);

/**
 * This exception is thrown when a function is passed an illegal or
 * inappropriate argument
 *
 * @c #IllegalArgumentException is thrown by a function when it detects that some
 * of the function parameters (passed by the caller) is unacceptable.
 *
 * @par     Extends:
 *          #RuntimeException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(IllegalArgumentException);

/**
 * This exception is thrown when an assertion does not hold
 *
 * @c #AssertionException is part of the assertion facility of the library. It is
 * thrown when the @e compile-time parameter @c NDEBUG is present and the
 * conditon of an assertion evaluates to @c false.
 *
 * @remark
 * This exception cannot be caught whatsoever. The program (or current thread)
 * will be terminated, after the execution of all pending @c #finally blocks.
 *
 * @see     #assert
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(AssertionException);

/**
 * This exception is thrown when an input/output error occurs
 *
 * @c #InputOutputException is the general type of exceptions produced by failed
 * or interrupted I/O operations.
 *
 * @par     Extends:
 *          #RuntimeException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(InputOutputException);

/**
 * This exception is the common supertype of all signal exceptions
 *
 * Signal exceptions are thrown when some signal is sent to the process.
 *
 * A signal can be generated by calling @c raise.
 *
 * @par     Extends:
 *          #RuntimeException
 *
 * @par     Direct known subexceptions:
 *          #SignalException,
 *          #SignalAlarmException,
 *          #SignalChildException,
 *          #SignalTrapException,
 *          #ErrorSignalException,
 *          #ControlSignalException,
 *          #ProgramSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(SignalException);

/**
 * This exception is thrown when a time limit has elapsed
 *
 * @c #SignalAlarmException represents @c SIGALRM, the signal sent to a process
 * when a time limit has elapsed.
 *
 * @par     Extends:
 *          #SignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(SignalAlarmException);

/**
 * This exception is thrown when a child process terminates
 *
 * @c #SignalChildException represents @c SIGCHLD, the signal sent to a process
 * when a child process terminates.
 *
 * @par     Extends:
 *          #SignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(SignalChildException);

/**
 * This exception is thrown when a condition arises that a debugger has
 * requested to be informed of
 *
 * @c #SignalTrapException represents @c SIGTRAP, the signal sent to a process
 * when a condition arises that a debugger has requested to be informed of.
 *
 * @par     Extends:
 *          #SignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(SignalTrapException);

/**
 * This exception is the common supertype of all error signal exceptions
 *
 * Error signal exceptions are thrown when some error prevents the program to
 * keep executing its normal flow.
 *
 * @par     Extends:
 *          #SignalException
 *
 * @par     Direct known subexceptions:
 *          #IllegalInstructionException,
 *          #BadPointerException,
 *          #ArithmeticException,
 *          #BrokenPipeException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(ErrorSignalException);

/**
 * This exception is thrown when the process attempts to execute an illegal
 * instruction
 *
 * @c #IllegalInstructionException represents @c SIGILL, the signal sent to a
 * process when it attempts to execute a malformed, unknown, or privileged
 * instruction.
 *
 * @par     Extends:
 *          #ErrorSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(IllegalInstructionException);

/**
 * This exception is thrown when the process performs an erroneous arithmetic
 * operation
 *
 * @c #ArithmeticException represents @c SIGFPE, the signal sent to a process
 * when it performs an erroneous arithmetic operation.
 *
 * @par     Extends:
 *          #ErrorSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(ArithmeticException);

/**
 * This exception is thrown when the process attempts to write to a broken pipe
 *
 * @c #BrokenPipeException represents @c SIGPIPE, the signal sent to a process
 * when it attempts to write to a pipe without a process connected to the other
 * end.
 *
 * @par     Extends:
 *          #ErrorSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(BrokenPipeException);

/**
 * This exception is thrown when the process tries to dereference an invalid
 * pointer
 *
 * @c #BadPointerException represents @c SIGSEGV, the signal sent to a process
 * when it makes an invalid memory reference, or segmentation fault.
 *
 * @par     Extends:
 *          #ErrorSignalException
 *
 * @par     Direct known subexceptions:
 *          #NullPointerException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(BadPointerException);

/**
 * This exception is thrown when an unexpected null pointer is found
 *
 * @c #NullPointerException is thrown when some part of the program gets a
 * pointer which was expected or required to contain a valid memory address.
 *
 * A @e null pointer exception is a special case of a @e bad pointer exception.
 * The difference between them is that @c #NullPointerException needs to be
 * thrown @e explicitly by some function, while @c #BadPointerException is
 * thrown @e implicitly by the signal handling system.
 *
 * @note
 * Sometimes, a null pointer exception can also be considered as a special case
 * of an <em>illegal argument</em> exception.
 *
 * @par     Extends:
 *          #BadPointerException
 *
 * @see     #IllegalArgumentException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(NullPointerException);

/**
 * This exception is the common supertype of all control signal exceptions
 *
 * Control signal exceptions are thrown when the process needs to be controlled
 * by the user or some other process.
 *
 * @par     Extends:
 *          #SignalException
 *
 * @par     Direct known subexceptions:
 *          #StopException,
 *          #KillException,
 *          #HangUpException,
 *          #TerminationException,
 *          #AbortException,
 *          #CPUTimeException,
 *          #UserControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(ControlSignalException);

/**
 * This exception is thrown to stop the process for later resumption
 *
 * @c #StopException represents @c SIGSTOP, the signal sent to a process to stop
 * it for later resumption.
 *
 * @remark
 * Since @c SIGSTOP is usually unblock-able, it won't be handled and converted
 * to this exception automatically on some platforms.
 *
 * @par     Extends:
 *          #ControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(StopException);

/**
 * This exception is thrown to terminate the process immediately
 *
 * @c #KillException represents @c SIGKILL, the signal sent to a process to
 * cause it to terminate immediately.
 *
 * @remark
 * Since @c SIGKILL is usually unblock-able, it won't be handled and converted
 * to this exception automatically on some platforms.
 *
 * @par     Extends:
 *          #ControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(KillException);

/**
 * This exception is thrown when the process' terminal is closed
 *
 * @c #HangUpException represents @c SIGHUP, the signal sent to a process when
 * its controlling terminal is closed.
 *
 * @par     Extends:
 *          #ControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(HangUpException);

/**
 * This exception is thrown to request the termination of the process
 *
 * @c #TerminationException represents @c SIGTERM, the signal sent to a process
 * to request its termination.
 *
 * @par     Extends:
 *          #ControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(TerminationException);

/**
 * This exception is thrown to abort the process
 *
 * @c #AbortException represents @c SIGABRT, the signal sent by computer
 * programs to abort the process.
 *
 * @par     Extends:
 *          #ControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(AbortException);

/**
 * This exception is thrown when the process has used up the CPU for too long
 *
 * @c #CPUTimeException represents @c SIGXCPU, the signal sent to a process when
 * it has used up the CPU for a duration that exceeds a certain predetermined
 * user-settable value.
 *
 * @par     Extends:
 *          #ControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(CPUTimeException);

/**
 * This exception is the common supertype of all control signal exceptions
 * caused by the user
 *
 * User control signal exceptions are thrown when the process needs to be
 * controlled by the user.
 *
 * @par     Extends:
 *          #ControlSignalException
 *
 * @par     Direct known subexceptions:
 *          #UserQuitException,
 *          #UserInterruptionException,
 *          #UserBreakException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(UserControlSignalException);

/**
 * This exception is thrown when the user requests to quit the process
 *
 * @c #UserQuitException represents @c SIGQUIT, the signal sent to a process by
 * its controlling terminal when the user requests that the process dump core.
 *
 * @par     Extends:
 *          #UserControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(UserQuitException);

/**
 * This exception is thrown when the user requests to interrupt the process
 *
 * @c #UserInterruptionException represents @c SIGINT, the signal sent to a
 * process by its controlling terminal when a user wishes to interrupt it.
 *
 * @par     Extends:
 *          #UserControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(UserInterruptionException);

/**
 * This exception is thrown when a user wishes to break the process
 *
 * @c #UserBreakException represents @c SIGBREAK, the signal sent to a process
 * by its controlling terminal when a user wishes to break it.
 *
 * @par     Extends:
 *          #UserControlSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(UserBreakException);

/**
 * This exception is the common supertype of all user-defined signal exceptions
 *
 * User-defined signal exceptions are thrown to indicate user-defined
 * conditions.
 *
 * @par     Extends:
 *          #SignalException
 *
 * @par     Direct known subexceptions:
 *          #ProgramSignal1Exception,
 *          #ProgramSignal2Exception
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(ProgramSignalException);

/**
 * This exception is thrown when user-defined conditions occur
 *
 * @c #ProgramSignal1Exception represents @c SIGUSR1, the signal sent to a
 * process to indicate user-defined conditions.
 *
 * @par     Extends:
 *          #ProgramSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(ProgramSignal1Exception);

/**
 * This exception is thrown when user-defined conditions occur
 *
 * @c #ProgramSignal2Exception represents @c SIGUSR1, the signal sent to a
 * process to indicate user-defined conditions.
 *
 * @par     Extends:
 *          #ProgramSignalException
 */
/*@unused@*/
E4C_DECLARE_EXCEPTION(ProgramSignal2Exception);

/** @} */

/**
 * @name Exception context handling functions
 *
 * These functions enclose the scope of the exception handling system and
 * retrieve the current exception context.
 *
 * @{
 */

/**
 * Checks if the current exception context is ready
 *
 * @return  Whether the current exception context of the program (or current
 *          thread) is ready to be used.
 *
 * This function returns whether there is an actual exception context ready to
 * be used by the program or current thread.
 *
 * @see     #e4c_context_begin
 * @see     #e4c_context_end
 * @see     #e4c_using_context
 * @see     #e4c_reusing_context
 */
/*@unused@*/ extern
E4C_BOOL
e4c_context_is_ready(
	void
)
# ifdef E4C_THREADSAFE
/*@globals
	fileSystem,
	internalState
*/
/*@modifies
	fileSystem,
	internalState
*/
# else
/*@globals
	internalState
@*/
# endif
;

/**
 * Begins an exception context
 *
 * @param   handle_signals
 *          If @c true, the signal handling system will be set up with the
 *          default mapping.
 *
 * This function begins the current exception context to be used by the program
 * (or current thread), until @c #e4c_context_end is called.
 *
 * The signal handling system can be initialized automatically with the default
 * signal mapping through the parameter @c handle_signals. This is equivalent
 * to:
 *
 * @code
 * e4c_context_set_signal_mappings(e4c_default_signal_mappings);
 * @endcode
 *
 * @warning
 * Note that, on some specific platforms, the behavior of the standard @c signal
 * function might be undefined for a multithreaded program, so use the signal
 * handling system with caution.
 *
 * The convenience function @c #e4c_print_exception will be used as the default
 * <em>uncaught handler</em>. It will be called in the event of an uncaught
 * exception, before exiting the program or thread. This handler can be set
 * through the function @c #e4c_context_set_handlers.
 *
 * @pre     Once @c #e4c_context_begin is called, the program (or thread)
 *          @b must call @c #e4c_context_end before exiting.
 * @pre     Calling @c #e4c_context_begin <em>twice in a row</em> is considered
 *          a programming error, and therefore the program (or thread) will exit
 *          abruptly. Nevertheless, @c #e4c_context_begin can be called several
 *          times <em>if, and only if,</em> @c #e4c_context_end is called in
 *          between.
 *
 * @see     #e4c_context_end
 * @see     #e4c_context_is_ready
 * @see     #e4c_using_context
 * @see     #e4c_reusing_context
 * @see     #e4c_uncaught_handler
 * @see     #e4c_print_exception
 * @see     #e4c_context_set_handlers
 */
/*@unused@*/ extern
void
e4c_context_begin(
	E4C_BOOL					handle_signals
)
/*@globals
	fileSystem,
	internalState,

	e4c_default_signal_mappings,

	NotEnoughMemoryException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/**
 * Ends the current exception context
 *
 * This function ends the current exception context.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          calling @c #e4c_context_end. Such programming error will lead to an
 *          abrupt exit of the program (or thread).
 *
 * @see     #e4c_context_begin
 * @see     #e4c_context_is_ready
 * @see     #e4c_using_context
 * @see     #e4c_reusing_context
 */
/*@unused@*/ extern
void
e4c_context_end(
	void
)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/**
 * Sets the optional handlers of an exception context
 *
 * @param   uncaught_handler
 *          The function to be executed in the event of an uncaught exception
 * @param   custom_data
 *          The initial value assigned to the custom_data of a new exception
 * @param   initialize_handler
 *          The function to be executed whenever a new exception is thrown
 * @param   finalize_handler
 *          The function to be executed whenever an exception is destroyed
 *
 * These handlers are a means of customizing the behavior of the exception
 * system. For example, you can specify what needs to be done when a thrown
 * exception is not caught (and thus, the program or thread is about to end) by
 * calling @c #e4c_context_set_handlers with the @c #e4c_uncaught_handler
 * function of your choice.
 *
 * You can also control the @ref e4c_exception::custom_data "custom data"
 * attached to any new exception (whether it is caught or not) by specifying any
 * or all of these:
 *
 * @li The initial value to be assigned to the @c custom_data
 * @li The funtion to @e initialize the @c custom_data
 * @li The funtion to @e finalize the @c custom_data
 *
 * When these handlers are defined, they will be called anytime an exception is
 * uncaught, created or destroyed. You can use them to meet your specific needs.
 * For example, you could...
 *
 * @li ...send an e-mail whenever an exception is uncaught
 * @li ...log any thrown exception to a file
 * @li ...capture the call stack in order to print it later on
 * @li ...go for something completely different ;)
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          calling @c #e4c_context_set_handlers. Such programming error will
 *          lead to an abrupt exit of the program (or thread).
 *
 * @see     #e4c_uncaught_handler
 * @see     #e4c_initialize_handler
 * @see     #e4c_finalize_handler
 * @see     #e4c_exception
 * @see     #e4c_print_exception
 */
/*@unused@*/ extern
void
e4c_context_set_handlers(
	/*@dependent@*/ /*@null@*/
	e4c_uncaught_handler uncaught_handler,
	/*@dependent@*/ /*@null@*/
	void * custom_data,
	/*@dependent@*/ /*@null@*/
	e4c_initialize_handler initialize_handler,
	/*@dependent@*/ /*@null@*/
	e4c_finalize_handler finalize_handler
)/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/;

/**
 * Assigns the specified signal mappings to the exception context
 *
 * @param   mappings
 *          The array of mappings
 *
 * This function assigns an array of mappings between the signals to be handled
 * and the corresponding exception to be thrown.
 *
 * @warning
 * Note that, on some specific platforms, the behavior of the standard @c signal
 * function might be undefined for a multithreaded program, so use the signal
 * handling system with caution.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          calling @c #e4c_context_set_signal_mappings. Such programming error
 *          will lead to an abrupt exit of the program (or thread).
 * @pre     @c mappings @b must be terminated by @c #E4C_NULL_SIGNAL_MAPPING.
 *
 * @see     #e4c_context_get_signal_mappings
 * @see     #e4c_signal_mapping
 * @see     #e4c_default_signal_mappings
 */
/*@unused@*/ extern
void
e4c_context_set_signal_mappings(
	/*@dependent@*/ /*@null@*/
	const e4c_signal_mapping * mappings
)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/**
 * Retrieves the signal mappings for the current exception context
 *
 * @return  The current array of mappings
 *
 * This function retrieves the current array of mappings between the signals to
 * be handled and the corresponding exception to be thrown.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          calling @c #e4c_context_get_signal_mappings. Such programming error
 *          will lead to an abrupt exit of the program (or thread).
 *
 * @see     #e4c_context_set_signal_mappings
 * @see     #e4c_signal_mapping
 * @see     #e4c_default_signal_mappings
 */
/*@unused@*/ extern
/*@observer@*/ /*@null@*/
const e4c_signal_mapping *
e4c_context_get_signal_mappings(
	void
)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/**
 * Returns the completeness status of the executing code block
 *
 * @return  The completeness status of the executing code block
 *
 * Exception-aware code blocks have a completeness status regarding the
 * exception handling system. This status determines whether an exception was
 * actually thrown or not, and whether the exception was caught or not.
 *
 * The status of the current block can be obtained any time, provided that the
 * exception context has begun at the time of the function call. However, it is
 * sensible to call this function only during the execution of @c #finally
 * blocks.
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          calling @c #e4c_get_status. Such programming error will lead to an
 *          abrupt exit of the program (or thread).
 *
 * @see     #e4c_get_status
 * @see     #finally
 */
/*@unused@*/ extern
e4c_status
e4c_get_status(
	void
)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/;

/**
 * Returns the exception that was thrown
 *
 * @return  The exception that was thrown in the current exception context (if
 *          any) otherwise @c NULL
 *
 * This function returns a pointer to the exception that was thrown in the
 * surrounding @e exception-aware block, if any; otherwise @c NULL.
 *
 * The function @c #e4c_is_instance_of will determine if the thrown exception is
 * an instance of any of the defined exception types. The @c type of the thrown
 * exception can also be compared for an exact type match.
 *
 * @code
 * try{
 *    ...
 * }catch(RuntimeException){
 *    const e4c_exception * exception = e4c_get_exception();
 *    if( e4c_is_instance_of(exception, &SignalException) ){
 *        ...
 *    }else if(exception->type == &NotEnoughMemoryException){
 *        ...
 *    }
 * }
 * @endcode
 *
 * The thrown exception can be obtained any time, provided that the exception
 * context has begun at the time of the function call. However, it is sensible
 * to call this function only during the execution of @c #finally or @c #catch
 * blocks.
 *
 * Moreover, a pointer to the thrown exception obtained @e inside a @c #finally
 * or @c #catch block <strong>must not</strong> be used from the @e outside.
 *
 * The exception system objects are dinamically allocated and deallocated, as
 * the program enters or exits @c #try... @c #catch... @c #finally blocks. While
 * it is legal to @e copy the thrown exception and access to its @c name and
 * @c message outside these blocks, care should be taken in order not to
 * dereference the @c cause of the exception, unless it is a <strong>deep
 * copy</strong> (as opposed to a <strong>shallow copy</strong>).
 *
 * @pre     A program (or thread) @b must begin an exception context prior to
 *          calling @c #e4c_get_exception. Such programming error will lead to
 *          an abrupt exit of the program (or thread).
 *
 * @see     #e4c_exception
 * @see     #e4c_is_instance_of
 * @see     #throw
 * @see     #catch
 * @see     #finally
 */
/*@unused@*/ extern
/*@observer@*/ /*@relnull@*/
const e4c_exception *
e4c_get_exception(
	void
)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/** @} */

/**
 * @name Other integration and convenience functions
 *
 * @{
 */

/**
 * Gets the library version number
 *
 * @return  The version number associated with the library
 *
 * This function provides the same information as the @c #E4C_VERSION_NUMBER
 * macro, but the returned version number is associated with the actual,
 * compiled library.
 *
 * @note
 * This version number can be considered as the @e run-time library version
 * number, as opposed to the @e compile-time library version number (specified
 * by the header file).
 *
 * @remark
 * The library @b must be compiled with the corresponding header (i.e. library
 * version number should be equal to header version number).
 *
 * @see     #E4C_VERSION_NUMBER
 */
/*@unused@*/ extern
long
e4c_library_version(
	void
)
/*@*/
;

/**
 * Returns whether an exception instance is of a given type
 *
 * @param   instance
 *          The thrown exception
 * @param   exception_type
 *          A previously defined type of exceptions
 * @return  Whether the specified exception is an instance of the given type
 *
 * @c #e4c_is_instance_of can be used to determine if a thrown exception
 * <strong>is an instance of</strong> a given exception type.
 *
 * This function is intended to be used in a @c #catch block, or in a @c
 * #finally block provided that some exception was actually thrown (i.e.
 * @c #e4c_get_status returned @c #e4c_failed or @c #e4c_recovered).
 *
 * This function will return @c false if either @c instance or @ type are
 * @c NULL.
 *
 * @code
 * try{
 *    ...
 * }catch(RuntimeException){
 *    const e4c_exception * exception = e4c_get_exception();
 *    if( e4c_is_instance_of(exception, &SignalException) ){
 *        ...
 *    }else if(exception->type == &NotEnoughMemoryException){
 *        ...
 *    }
 * }
 * @endcode
 *
 * @pre     @c instance <strong>must not</strong> be @c NULL
 * @pre     @c type <strong>must not</strong> be @c NULL
 *
 * @see     #e4c_exception
 * @see     #e4c_exception_type
 * @see     #e4c_get_exception
 */
/*@unused@*/ extern
E4C_BOOL
e4c_is_instance_of(
	/*@temp@*/ /*@notnull@*/
	const e4c_exception *		instance,
	/*@temp@*/ /*@notnull@*/
	const e4c_exception_type *	exception_type
)
/*@*/
;

/**
 * Prints a fatal error message regarding the specified exception
 *
 * @param   exception
 *          The uncaught exception
 *
 * This is a convenience function for showing an error message through the
 * standard error output. It can be passed to @c #e4c_context_set_handlers as
 * the handler for uncaught exceptions, and will be used, by default, unless
 * otherwise set up.
 *
 * In absence of @c NDEBUG, this function prints as much information regarding
 * the exception as it is available, whereas in presence of @c NDEBUG, only the
 * @c name and @c message of the exception are printed.
 *
 * @pre     @c exception <strong>must not</strong> be @c NULL
 * @throws  #NullPointerException
 *          If @c exception is @c NULL
 *
 * @see     #e4c_uncaught_handler
 * @see     #e4c_context_begin
 * @see     #e4c_using_context
 */
/*@unused@*/ extern
void
e4c_print_exception(
	/*@temp@*/ /*@notnull@*/
	const e4c_exception *		exception
)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/**
 * Prints an ASCII graph representing an exception type's hierarchy
 *
 * @param   exception_type
 *          An exception type
 *
 * This is a convenience function for showing an ASCII graph representing an
 * exception type's hierarchy through the standard error output.
 *
 * For example, the output for @c #ProgramSignal2Exception would be:
 *
@verbatim

	Exception hierarchy
	________________________________________________________________

	    RuntimeException
	     |
	     +--SignalException
	         |
	         +--ProgramSignalException
	             |
	             +--ProgramSignal2Exception
	________________________________________________________________

@endverbatim
 *
 * @pre     @c exception_type <strong>must not</strong> be @c NULL
 * @throws  #NullPointerException
 *          If @c exception_type is @c NULL
 *
 * @see     #e4c_exception_type
 */
/*@unused@*/ extern
void
e4c_print_exception_type(
	/*@shared@*/ /*@notnull@*/
	const e4c_exception_type *	exception_type
)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/** @} */

/*
 * Next functions are undocumented on purpose, because they shouldn't be used
 * directly (but through the "keywords").
 */

/*@unused@*/ extern
/*@notnull@*/ /*@temp@*/
struct e4c_continuation_ *
e4c_frame_first_stage_(
	enum e4c_frame_stage_		stage,
	/*@observer@*/ /*@null@*/
	const char *				file,
	int							line,
	/*@observer@*/ /*@null@*/
	const char *				function
)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/*@unused@*/ extern
E4C_BOOL
e4c_frame_next_stage_(
	void
)
/*@globals
	fileSystem,
	internalState,

	AssertionException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/*@unused@*/ extern
enum e4c_frame_stage_
e4c_frame_get_stage_(
	/*@observer@*/ /*@null@*/
	const char *				file,
	int							line,
	/*@observer@*/ /*@null@*/
	const char *				function
)
/*@globals
	fileSystem,
	internalState
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/*@unused@*/ extern
E4C_BOOL
e4c_frame_catch_(
	/*@temp@*/ /*@null@*/
	const e4c_exception_type *	exception_type,
	/*@observer@*/ /*@null@*/
	const char *				file,
	int							line,
	/*@observer@*/ /*@null@*/
	const char *				function
)
/*@globals
	fileSystem,
	internalState,

	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/*@unused@*/ /*@maynotreturn@*/ extern
void
e4c_frame_repeat_(
	int							max_repeat_attempts,
	enum e4c_frame_stage_		stage,
	/*@observer@*/ /*@null@*/
	const char *				file,
	int							line,
	/*@observer@*/ /*@null@*/
	const char *				function
)
/*@globals
	fileSystem,
	internalState,

	AssertionException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
;

/*@unused@*/ /*@noreturn@*/ extern
void
e4c_exception_throw_verbatim_(
	/*@shared@*/ /*@notnull@*/
	const e4c_exception_type *	exception_type,
	/*@observer@*/ /*@null@*/
	const char *				file,
	int							line,
	/*@observer@*/ /*@null@*/
	const char *				function,
	/*@observer@*/ /*@temp@*/ /*@null@*/
	const char *				message
)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
E4C_NO_RETURN;

# if defined(HAVE_C99_VSNPRINTF) || defined(HAVE_VSNPRINTF)

/*@unused@*/ /*@noreturn@*/ extern
void
e4c_exception_throw_format_(
	/*@shared@*/ /*@notnull@*/
	const e4c_exception_type *	exception_type,
	/*@observer@*/ /*@null@*/
	const char *				file,
	int							line,
	/*@observer@*/ /*@null@*/
	const char *				function,
	/*@observer@*/ /*@temp@*/ /*@notnull@*/ /*@printflike@*/
	const char *				format,
	...
)
/*@globals
	fileSystem,
	internalState,

	NotEnoughMemoryException,
	NullPointerException
@*/
/*@modifies
	fileSystem,
	internalState
@*/
E4C_NO_RETURN;

# endif

/*
 * End of the extern "C" block.
 */
#ifdef __cplusplus
	}
#endif

/*@=exportany@*/


# endif
