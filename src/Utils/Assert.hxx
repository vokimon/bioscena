/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _ASSERT_
#define _ASSERT_
#include <exception>

/** @file Assert.hxx
* Bug hunting and detection facilities for developers.
*/


namespace Bioscena
{

/** 
* @def KKEP_BREAKPOINT
* A macro containing break point assembler code for your platform.
* If you platform is not being considered by the header you will 
* get an warning message when compiling.
*/

// Microsoft VisualC++ and ancestror MSC
#ifdef _MSC_VER
#define KKEP_BREAKPOINT {_asm {int 3}}
	
// MetroWorks Code Warrior
#elif defined (__MWERKS__)
#define KKEP_BREAKPOINT {_asm {int 3}}

// GNU GCC
#elif defined (__GNUC__) && defined  (__i386__)
#define KKEP_BREAKPOINT {__asm__ (" int $3 "); }

#elif defined (__GNUC__) && defined  (__powerpc__)
#define KKEP_BREAKPOINT {__asm__ (" .long 0x7d821008 "); }

// Insert your compiler here
#else
#warning Breakpoint code unknown for the platform. You can add it defining the KKEP_BREAKPOINT macro at file __FILE__.
#define KKEP_BREAKPOINT {}
#endif


#if ! defined(_DEBUG)
#define KKEP_USE_RELEASE_ASSERTS
#endif

/// Macro used when an assert fails
#if defined(KKEP_USE_RELEASE_ASSERTS)
#define KKEP_ABORT(description) \
	{ \
		throw Bioscena::ErrAssertionFailed( description, __FILE__, __LINE__); \
	}
#else
#define KKEP_ABORT(description) \
	{ \
		Biscena::ExecuteAssertFailedHandler ( description, __FILE__, __LINE__); \
		KKEP_BREAKPOINT; \
	}
#endif

/// @name Assertions and checks
/// @{
/**
* @def KKEP_ASSERT(expresion,message)
* An assertion is a check on an expression that must be
* true given that all the contracts are fullfilled.
* Notice that this is NOT a regular (documented) check
* that will throw a (documented) error condition (Err).
* Asserts on debug mode (when the DEBUG macro is defined)
* will set a break point while in release mode (DEBUG is 
* not defined) you may get an unexpected exception that
* can be catched by the application on the top level and
* do some backups actions. 
* Any way, a descriptive message is displayed.
*
* Sometimes the check is not only an expression but a
* complete set of statements. In these cases, you can use 
* KKEP_BEGIN_CHECK and KKEP_END_CHECK macros to enclose
* the whole check code and use KKEP_ASSERT for concrete 
* expressions to check.
*
* You can change the default release mode assertion 
* failed callback by using the SetAssertFailedHandler 
* function.
*
* You can get a lightweight assertion using (with care)
* the KKEP_DEBUG_ASSERT that is ignored on debug mode.
* <b>By using the KKEP_DEBUG_ASSERT macro, the final
* application will not have the possibility of doing
* an honrous exit (backup data, bug log...)</b>
* Is worth to keep most of the asserts as KKEP_ASSERT.
*
* Anyway, for very performance depending applications
* you may use the KKEP_DISABLE_CHECKS to remove ALL the 
* KKEP_ASSERT and KKEP_DEBUG_ASSERT and its related checks 
* in whatever mode you use them.
* 
* @param expression The expression that must be true.
* @param message A message that describes the unexpected 
* runtime error to the programmer.
*/
/** 
* @def KKEP_BEGIN_CHECK
* Marks the start of check code block that is tied to a KKEP_ASSERT
* and can be removed without affecting any functionality.
* You must use it on the same sense that KKEP_ASSERT is used.
* @see KKEP_ASSERT
* @see KKEP_END_CHECK
*/
/** 
* @def KKEP_END_CHECK
* Ends a block of code started by KKEP_BEGIN_CHECK
* @see KKEP_BEGIN_CHECK
*/
#if defined(KKEP_DISABLE_CHECKS)
#define KKEP_BEGIN_CHECK if (0) {
#define KKEP_END_CHECK }
#define KKEP_ASSERT( expression, description ) 
#define KKEP_WARNING( expression, description )
#else
#define KKEP_BEGIN_CHECK {
#define KKEP_END_CHECK }
#define KKEP_ASSERT( expression, description ) \
	if (!(expression)) { \
		KKEP_ABORT(description); \
	}
#define KKEP_WARNING( expression, description ) \
	if (!(expression)) { \
		CLAM::ExecuteWarningHandler ( description, __FILE__, __LINE__); \
	}
#endif


/** 
* @def KKEP_BEGIN_DEBUG_CHECK
* Is the same that KKEP_BEGIN_CHECK but for a KKEP_DEBUG_ASSERT
* instead of a KKEP_ASSERT.
* @see KKEP_DEBUG_ASSERT
* @see KKEP_BEGIN_CHECK
* @see KKEP_END_DEBUG_CHECK
*/
/** 
* @def KKEP_END_DEBUG_CHECK
* Ends a block of code started by KKEP_BEGIN_DEBUG_CHECK
* @see KKEP_BEGIN_DEBUG_CHECK
*/

#if defined(KKEP_DISABLE_CHECKS) || defined(KKEP_USE_RELEASE_ASSERTS)
#define KKEP_BEGIN_DEBUG_CHECK if (0) {
#define KKEP_END_DEBUG_CHECK }
#define KKEP_DEBUG_ASSERT( expression, description ) 
#define KKEP_DEBUG_WARNING( expression, description )
#else
#define KKEP_BEGIN_DEBUG_CHECK {
#define KKEP_END_DEBUG_CHECK }
#define KKEP_DEBUG_ASSERT( expression, description ) \
	if (!(expression)) { \
		KKEP_ABORT(description); \
	}
#define KKEP_DEBUG_WARNING( expression, description ) \
	if (!(expression)) { \
		CLAM::ExecuteWarningHandler ( description, __FILE__, __LINE__); \
	}
#endif
/// @}

class Err : public std::exception {
public:
	Err() {loque="Indefinida";};
	Err(const char *msg) {loque=msg;};
	Err(const Err & e) {loque=e.what();};
	virtual ~Err() throw() {}
	const char * what() const throw() {return loque;}
private:
	const char * loque;
};

/**
* The exception thrown when an assertion fails.
* Don't throw it directly, use the assertion macro Assert instead
* because the Assert macro is sensible to the compilation conditions.
* Neither do any explicit declaration that this exception can
* be thrown from a function as you would have to do with any other 
* exception.
* See the Error Notification Mechanisms documentation.
* @todo Subclass ErrAssertionFailed from Err.
* @see KKEP_ASSERT
* @see KKEP_BEGIN_CHECK
* @see KKEP_END_CHECK
*/

class ErrAssertionFailed : public Err {
public:
	ErrAssertionFailed(const char* message, const char* filename, int linenumber);
	virtual ~ErrAssertionFailed() throw () {}
};

/**
* The type of the asserts handlers.
* @param message A char pointer containing a description of the assertion
* @param fileName A char pointer containing the source file where the assertion is placed
* @param lineNumber The line of the source file where the assertion is placed
*/
typedef void (*AssertFailedHandlerType) (const char* message, const char* filename, int lineNumber);
/**
* Change the handler function called when an assertion fails.
* The function must have a prototype like that:
* <pre>
* void MyHandler (const char* message, const char* filename, int lineNumber);
* </pre>
* @param The new handler
* @return The old handler, for restoring purposes.
*/
AssertFailedHandlerType SetAssertFailedHandler(AssertFailedHandlerType);

/**
* (Don't use directly, use the KKEP_ASSERT macro instead) Execute the assert failed handler.
*/
void ExecuteAssertFailedHandler(const char* message, const char* filename, int linenumber);

/**
* The type of the warning handlers.
* @param message A char pointer containing a description of the warning
* @param fileName A char pointer containing the source file where the warning is placed
* @param lineNumber The line of the source file where the warning is placed
*/
typedef void (*WarningHandlerType) (const char* message, const char* filename, int lineNumber);
/**
* Change the handler function called when a warning is given.
* The function must have a prototype like that:
* <pre>
* void MyHandler (const char* message, const char* filename, int lineNumber);
* </pre>
* @param The new handler
* @return The old handler, for restoring purposes.
*/
WarningHandlerType SetWarningHandler(WarningHandlerType);

/**
* (Don't use directly, use the KKEP_WARNING macro instead) Execute the assert failed handler.
*/
void ExecuteWarningHandler(const char* message, const char* filename, int linenumber);


}

#endif //_ASSERT_

