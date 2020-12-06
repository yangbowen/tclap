// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  CmdLine.h
 *
 *  Copyright (c) 2003, Michael E. Smoot .
 *  Copyright (c) 2004, Michael E. Smoot, Daniel Aarno.
 *  All rights reserved.
 *
 *  See the file COPYING in the top directory of this distribution for
 *  more information.
 *
 *  THE SOFTWARE IS PROVIDED _AS IS_, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

#ifndef TCLAP_CMDLINE_H
#define TCLAP_CMDLINE_H

#include <tclap/SwitchArg.h>
#include <tclap/MultiSwitchArg.h>
#include <tclap/UnlabeledValueArg.h>
#include <tclap/UnlabeledMultiArg.h>

#include <tclap/XorHandler.h>
#include <tclap/Visitor.h>
#include <tclap/HelpVisitor.h>
#include <tclap/VersionVisitor.h>
#include <tclap/IgnoreRestVisitor.h>

#include <tclap/CmdLineOutput.h>
#include <tclap/StdOutput.h>

#include <tclap/Constraint.h>
#include <tclap/ValuesConstraint.h>

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdlib.h> // Needed for exit(), which isn't defined in some envs.

namespace TCLAP {

template<typename T> void DelPtr(T ptr)
{
	delete ptr;
}

template<typename C> void ClearContainer(C &c)
{
	typedef typename C::value_type value_type;
	std::for_each(c.begin(), c.end(), DelPtr<value_type>);
	c.clear();
}


/**
 * The base class that manages the command line definition and passes
 * along the parsing to the appropriate Arg classes.
 */
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class CmdLine : public CmdLineInterface<T_Char, T_CharTraits, T_Alloc>
{
	public:
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::AllocatorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::StringType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::StringVectorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgListType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgVectorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgListIteratorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgVectorIteratorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::CmdLineInterfaceType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::CmdLineOutputType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::XorHandlerType;
		using VisitorListType = std::list<Visitor*, typename std::allocator_traits<AllocatorType>::template rebind_alloc<Visitor*>>;

	protected:

		/**
		 * The list of arguments that will be tested against the
		 * command line.
		 */
		ArgListType _argList;

		/**
		 * The name of the program.  Set to argv[0].
		 */
		StringType _progName;

		/**
		 * A message used to describe the program.  Used in the usage output.
		 */
		StringType _message;

		/**
		 * The version to be displayed with the --version switch.
		 */
		StringType _version;

		/**
		 * The number of arguments that are required to be present on
		 * the command line. This is set dynamically, based on the
		 * Args added to the CmdLine object.
		 */
		int _numRequired;

		/**
		 * The character that is used to separate the argument flag/name
		 * from the value.  Defaults to ' ' (space).
		 */
		CharType _delimiter;

		/**
		 * The handler that manages xoring lists of args.
		 */
		XorHandlerType _xorHandler;

		/**
		 * A list of Args to be explicitly deleted when the destructor
		 * is called.  At the moment, this only includes the three default
		 * Args.
		 */
		ArgListType _argDeleteOnExitList;

		/**
		 * A list of Visitors to be explicitly deleted when the destructor
		 * is called.  At the moment, these are the Visitors created for the
		 * default Args.
		 */
		VisitorListType _visitorDeleteOnExitList;

		/**
		 * Object that handles all output for the CmdLine.
		 */
		CmdLineOutputType* _output;

		/**
		 * Should CmdLine handle parsing exceptions internally?
		 */
		bool _handleExceptions;

		/**
		 * Throws an exception listing the missing args.
		 */
		void missingArgsException();

		/**
		 * Checks whether a name/flag string matches entirely matches
		 * the ArgType::blankChar.  Used when multiple switches are combined
		 * into a single argument.
		 * \param s - The message to be used in the usage.
		 */
		bool _emptyCombined(const StringType& s);

		/**
		 * Perform a delete ptr; operation on ptr when this object is deleted.
		 */
		void deleteOnExit(ArgType* ptr);

		/**
		 * Perform a delete ptr; operation on ptr when this object is deleted.
		 */
		void deleteOnExit(Visitor* ptr);

private:

		/**
		 * Prevent accidental copying.
		 */
		CmdLine(const CmdLine& rhs);
		CmdLine& operator=(const CmdLine& rhs);

		/**
		 * Encapsulates the code common to the constructors
		 * (which is all of it).
		 */
		void _constructor();


		/**
		 * Is set to true when a user sets the output object. We use this so
		 * that we don't delete objects that are created outside of this lib.
		 */
		bool _userSetOutput;

		/**
		 * Whether or not to automatically create help and version switches.
		 */
		bool _helpAndVersion;

		/**
		 * Whether or not to ignore unmatched args.
		 */
		bool _ignoreUnmatched;

	public:

		/**
		 * Command line constructor. Defines how the arguments will be
		 * parsed.
		 * \param message - The message to be used in the usage
		 * output.
		 * \param delimiter - The character that is used to separate
		 * the argument flag/name from the value.  Defaults to ' ' (space).
		 * \param version - The version number to be used in the
		 * --version switch.
		 * \param helpAndVersion - Whether or not to create the Help and
		 * Version switches. Defaults to true.
		 */
		CmdLine(const StringType& message,
				const CharType delimiter = ' ',
				const StringType& version = "none",
				bool helpAndVersion = true);

		/**
		 * Deletes any resources allocated by a CmdLine object.
		 */
		virtual ~CmdLine();

		/**
		 * Adds an argument to the list of arguments to be parsed.
		 * \param a - Argument to be added.
		 */
		void add( ArgType& a );

		/**
		 * An alternative add.  Functionally identical.
		 * \param a - Argument to be added.
		 */
		void add( ArgType* a );

		/**
		 * Add two Args that will be xor'd.  If this method is used, add does
		 * not need to be called.
		 * \param a - Argument to be added and xor'd.
		 * \param b - Argument to be added and xor'd.
		 */
		void xorAdd( ArgType& a, ArgType& b );

		/**
		 * Add a list of Args that will be xor'd.  If this method is used,
		 * add does not need to be called.
		 * \param xors - List of Args to be added and xor'd.
		 */
		void xorAdd( const ArgVectorType& xors );

		/**
		 * Parses the command line.
		 * \param argc - Number of arguments.
		 * \param argv - Array of arguments.
		 */
		void parse(int argc, const CharType * const * argv);

		/**
		 * Parses the command line.
		 * \param args - A vector of strings representing the args.
		 * args[0] is still the program name.
		 */
		void parse(StringVectorType& args);

		/**
		 *
		 */
		CmdLineOutputType* getOutput();

		/**
		 *
		 */
		void setOutput(CmdLineOutputType* co);

		/**
		 *
		 */
		StringType& getVersion();

		/**
		 *
		 */
		StringType& getProgramName();

		/**
		 *
		 */
		ArgListType& getArgList();

		/**
		 *
		 */
		XorHandlerType& getXorHandler();

		/**
		 *
		 */
		CharType getDelimiter();

		/**
		 *
		 */
		StringType& getMessage();

		/**
		 *
		 */
		bool hasHelpAndVersion();

		/**
		 * Disables or enables CmdLine's internal parsing exception handling.
		 *
		 * @param state Should CmdLine handle parsing exceptions internally?
		 */
		void setExceptionHandling(const bool state);

		/**
		 * Returns the current state of the internal exception handling.
		 *
		 * @retval true Parsing exceptions are handled internally.
		 * @retval false Parsing exceptions are propagated to the caller.
		 */
		bool getExceptionHandling() const;

		/**
		 * Allows the CmdLine object to be reused.
		 */
		void reset();

		/**
		 * Allows unmatched args to be ignored. By default false.
		 * 
		 * @param ignore If true the cmdline will ignore any unmatched args
		 * and if false it will behave as normal.
		 */
		void ignoreUnmatched(const bool ignore);
};


///////////////////////////////////////////////////////////////////////////////
//Begin CmdLine.cpp
///////////////////////////////////////////////////////////////////////////////

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline CmdLine<T_Char, T_CharTraits, T_Alloc>::CmdLine(const StringType& m,
                        CharType delim,
                        const StringType& v,
                        bool help )
    :
  _argList(ArgListType()),
  _progName("not_set_yet"),
  _message(m),
  _version(v),
  _numRequired(0),
  _delimiter(delim),
  _xorHandler(XorHandlerType()),
  _argDeleteOnExitList(ArgListType()),
  _visitorDeleteOnExitList(VisitorListType()),
  _output(0),
  _handleExceptions(true),
  _userSetOutput(false),
  _helpAndVersion(help),
  _ignoreUnmatched(false)
{
	_constructor();
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline CmdLine<T_Char, T_CharTraits, T_Alloc>::~CmdLine()
{
	ClearContainer(_argDeleteOnExitList);
	ClearContainer(_visitorDeleteOnExitList);

	if ( !_userSetOutput ) {
		delete _output;
		_output = 0;
	}
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::_constructor()
{
	_output = new StdOutput<T_Char, T_CharTraits, T_Alloc>;

	ArgType::setDelimiter( _delimiter );

	Visitor* v;

	if ( _helpAndVersion )
	{
		v = new HelpVisitor<T_Char, T_CharTraits, T_Alloc>( this, &_output );
		SwitchArg* help = new SwitchArg<T_Char, T_CharTraits, T_Alloc>("h","help",
		                      "Displays usage information and exits.",
		                      false, v);
		add( help );
		deleteOnExit(help);
		deleteOnExit(v);

		v = new VersionVisitor<T_Char, T_CharTraits, T_Alloc>( this, &_output );
		SwitchArg* vers = new SwitchArg<T_Char, T_CharTraits, T_Alloc>("","version",
		                      "Displays version information and exits.",
		                      false, v);
		add( vers );
		deleteOnExit(vers);
		deleteOnExit(v);
	}

	v = new IgnoreRestVisitor<T_Char, T_CharTraits, T_Alloc>();
	SwitchArg<T_Char, T_CharTraits, T_Alloc>* ignore  = new SwitchArg<T_Char, T_CharTraits, T_Alloc>(ArgType::flagStartString(),
	          ArgType::ignoreNameString(),
	          "Ignores the rest of the labeled arguments following this flag.",
	          false, v);
	add( ignore );
	deleteOnExit(ignore);
	deleteOnExit(v);
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::xorAdd( const ArgVectorType& ors )
{
	_xorHandler.add( ors );

	for (ArgVectorIterator it = ors.begin(); it != ors.end(); it++)
	{
		(*it)->forceRequired();
		(*it)->setRequireLabel( "OR required" );
		add( *it );
	}
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::xorAdd( ArgType& a, ArgType& b )
{
	ArgVectorType ors;
	ors.push_back( &a );
	ors.push_back( &b );
	xorAdd( ors );
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::add( ArgType& a )
{
	add( &a );
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::add( ArgType* a )
{
	for( ArgListIteratorType it = _argList.begin(); it != _argList.end(); it++ )
		if ( *a == *(*it) )
			throw( SpecificationException(
			        "Argument with same flag/name already exists!",
			        a->longID() ) );

	a->addToList( _argList );

	if ( a->isRequired() )
		_numRequired++;
}


template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::parse(int argc, const CharType * const * argv)
{
		// this step is necessary so that we have easy access to
		// mutable strings.
		StringVectorType args;
		for (int i = 0; i < argc; i++)
			args.push_back(argv[i]);

		parse(args);
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::parse(StringVectorType& args)
{
	bool shouldExit = false;
	int estat = 0;

	try {
		_progName = args.front();
		args.erase(args.begin());

		int requiredCount = 0;

		for (int i = 0; static_cast<unsigned int>(i) < args.size(); i++) 
		{
			bool matched = false;
			for (ArgListIterator it = _argList.begin();
			     it != _argList.end(); it++) {
				if ( (*it)->processArg( &i, args ) )
				{
					requiredCount += _xorHandler.check( *it );
					matched = true;
					break;
				}
			}

			// checks to see if the argument is an empty combined
			// switch and if so, then we've actually matched it
			if ( !matched && _emptyCombined( args[i] ) )
				matched = true;

			if ( !matched && !ArgType::ignoreRest() && !_ignoreUnmatched)
				throw(CmdLineParseException("Couldn't find match "
				                            "for argument",
				                            args[i]));
		}

		if ( requiredCount < _numRequired )
			missingArgsException();

		if ( requiredCount > _numRequired )
			throw(CmdLineParseException("Too many arguments!"));

	} catch ( ArgException& e ) {
		// If we're not handling the exceptions, rethrow.
		if ( !_handleExceptions) {
			throw;
		}

		try {
			_output->failure(*this,e);
		} catch ( ExitException &ee ) {
			estat = ee.getExitStatus();
			shouldExit = true;
		}
	} catch (ExitException &ee) {
		// If we're not handling the exceptions, rethrow.
		if ( !_handleExceptions) {
			throw;
		}

		estat = ee.getExitStatus();
		shouldExit = true;
	}

	if (shouldExit)
		exit(estat);
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline bool CmdLine<T_Char, T_CharTraits, T_Alloc>::_emptyCombined(const StringType& s)
{
	if ( s.length() > 0 && s[0] != ArgType::flagStartChar() )
		return false;

	for ( int i = 1; static_cast<unsigned int>(i) < s.length(); i++ )
		if ( s[i] != ArgType::blankChar() )
			return false;

	return true;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::missingArgsException()
{
		int count = 0;

		StringType missingArgList;
		for (ArgListIterator it = _argList.begin(); it != _argList.end(); it++)
		{
			if ( (*it)->isRequired() && !(*it)->isSet() )
			{
				missingArgList += (*it)->getName();
				missingArgList += ", ";
				count++;
			}
		}
		missingArgList = missingArgList.substr(0,missingArgList.length()-2);

		StringType msg;
		if ( count > 1 )
			msg = "Required arguments missing: ";
		else
			msg = "Required argument missing: ";

		msg += missingArgList;

		throw(CmdLineParseException(msg));
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::deleteOnExit(ArgType* ptr)
{
	_argDeleteOnExitList.push_back(ptr);
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::deleteOnExit(Visitor* ptr)
{
	_visitorDeleteOnExitList.push_back(ptr);
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getOutput() -> CmdLineOutputType*
{
	return _output;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::setOutput(CmdLineOutputType* co)
{
	if ( !_userSetOutput )
		delete _output;
	_userSetOutput = true;
	_output = co;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getVersion() -> StringType&
{
	return _version;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getProgramName() -> StringType&
{
	return _progName;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getArgList() -> ArgListType&
{
	return _argList;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getXorHandler() -> XorHandlerType&
{
	return _xorHandler;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getDelimiter() -> CharType
{
	return _delimiter;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getMessage() -> StringType&
{
	return _message;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline bool CmdLine<T_Char, T_CharTraits, T_Alloc>::hasHelpAndVersion()
{
	return _helpAndVersion;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::setExceptionHandling(const bool state)
{
	_handleExceptions = state;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline bool CmdLine<T_Char, T_CharTraits, T_Alloc>::getExceptionHandling() const
{
	return _handleExceptions;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::reset()
{
	for( ArgListIterator it = _argList.begin(); it != _argList.end(); it++ )
		(*it)->reset();
	
	_progName.clear();
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::ignoreUnmatched(const bool ignore)
{
	_ignoreUnmatched = ignore;
}

///////////////////////////////////////////////////////////////////////////////
//End CmdLine.cpp
///////////////////////////////////////////////////////////////////////////////



} //namespace TCLAP
#endif
