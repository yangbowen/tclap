// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/****************************************************************************** 
 * 
 *  file:  DocBookOutput.h
 * 
 *  Copyright (c) 2004, Michael E. Smoot
 *  Copyright (c) 2017, Google LLC
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

#ifndef TCLAP_DOCBOOKOUTPUT_H
#define TCLAP_DOCBOOKOUTPUT_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

#include <tclap/CmdLineInterface.h>
#include <tclap/CmdLineOutput.h>
#include <tclap/XorHandler.h>
#include <tclap/Arg.h>

namespace TCLAP {

/**
 * A class that generates DocBook output for usage() method for the 
 * given CmdLine and its Args.
 */
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class DocBookOutput : public CmdLineOutput<T_Char, T_CharTraits, T_Alloc>
{
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::StringType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::StringVectorType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::ArgType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::ArgListType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::ArgVectorType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::ArgVectorVectorType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::ArgListIteratorType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::ArgVectorIteratorType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::CmdLineInterfaceType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::CmdLineOutputType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::XorHandlerType;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		/**
		 * Prints the usage to stdout.  Can be overridden to 
		 * produce alternative behavior.
		 * \param c - The CmdLine object the output is generated for. 
		 */
		virtual void usage(CmdLineInterfaceType& c);

		/**
		 * Prints the version to stdout. Can be overridden 
		 * to produce alternative behavior.
		 * \param c - The CmdLine object the output is generated for. 
		 */
		virtual void version(CmdLineInterfaceType& c);

		/**
		 * Prints (to stderr) an error message, short usage 
		 * Can be overridden to produce alternative behavior.
		 * \param c - The CmdLine object the output is generated for. 
		 * \param e - The ArgException that caused the failure. 
		 */
		virtual void failure(CmdLineInterfaceType& c,
						     ArgException& e );

	    DocBookOutput(const AllocatorType& alloc = AllocatorType()) : CmdLineOutput<T_Char, T_CharTraits, T_Alloc>(alloc), theDelimiter('=') {}
	protected:

		/**
		 * Substitutes the char r for string x in string s.
		 * \param s - The string to operate on. 
		 * \param r - The char to replace. 
		 * \param x - What to replace r with. 
		 */
		void substituteSpecialChars( StringType& s, CharType r, StringType& x );
		void removeChar( StringType& s, CharType r);
		void basename( StringType& s );

		void printShortArg(ArgType* it);
		void printLongArg(ArgType* it);

		CharType theDelimiter;
};


template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::version(CmdLineInterfaceType& _cmd)
{ 
	std::cout << _cmd.getVersion() << std::endl;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::usage(CmdLineInterfaceType& _cmd )
{
	ArgListType argList = _cmd.getArgList();
	StringType progName = _cmd.getProgramName();
	StringType xversion = _cmd.getVersion();
	theDelimiter = _cmd.getDelimiter();
	XorHandlerType xorHandler = _cmd.getXorHandler();
	const ArgVectorVectorType xorList = xorHandler.getXorList();
	basename(progName);

	std::cout << "<?xml version='1.0'?>" << std::endl;
	std::cout << "<!DOCTYPE refentry PUBLIC \"-//OASIS//DTD DocBook XML V4.2//EN\"" << std::endl;
	std::cout << "\t\"http://www.oasis-open.org/docbook/xml/4.2/docbookx.dtd\">" << std::endl << std::endl;

	std::cout << "<refentry>" << std::endl;

	std::cout << "<refmeta>" << std::endl;
	std::cout << "<refentrytitle>" << progName << "</refentrytitle>" << std::endl;
	std::cout << "<manvolnum>1</manvolnum>" << std::endl;
	std::cout << "</refmeta>" << std::endl;

	std::cout << "<refnamediv>" << std::endl;
	std::cout << "<refname>" << progName << "</refname>" << std::endl;
	std::cout << "<refpurpose>" << _cmd.getMessage() << "</refpurpose>" << std::endl;
	std::cout << "</refnamediv>" << std::endl;

	std::cout << "<refsynopsisdiv>" << std::endl;
	std::cout << "<cmdsynopsis>" << std::endl;

	std::cout << "<command>" << progName << "</command>" << std::endl;

	// xor
	for ( int i = 0; (unsigned int)i < xorList.size(); i++ )
	{
		std::cout << "<group choice='req'>" << std::endl;
		for ( ArgVectorIteratorType it = xorList[i].begin(); 
						it != xorList[i].end(); it++ )
			printShortArg((*it));

		std::cout << "</group>" << std::endl;
	}

	// rest of args
	for (ArgListIteratorType it = argList.begin(); it != argList.end(); it++)
		if ( !xorHandler.contains( (*it) ) )
			printShortArg((*it));

 	std::cout << "</cmdsynopsis>" << std::endl;
	std::cout << "</refsynopsisdiv>" << std::endl;

	std::cout << "<refsect1>" << std::endl;
	std::cout << "<title>Description</title>" << std::endl;
	std::cout << "<para>" << std::endl;
	std::cout << _cmd.getMessage() << std::endl; 
	std::cout << "</para>" << std::endl;
	std::cout << "</refsect1>" << std::endl;

	std::cout << "<refsect1>" << std::endl;
	std::cout << "<title>Options</title>" << std::endl;

	std::cout << "<variablelist>" << std::endl;
	
	for (ArgListIteratorType it = argList.begin(); it != argList.end(); it++)
		printLongArg((*it));

	std::cout << "</variablelist>" << std::endl;
	std::cout << "</refsect1>" << std::endl;

	std::cout << "<refsect1>" << std::endl;
	std::cout << "<title>Version</title>" << std::endl;
	std::cout << "<para>" << std::endl;
	std::cout << xversion << std::endl; 
	std::cout << "</para>" << std::endl;
	std::cout << "</refsect1>" << std::endl;
	
	std::cout << "</refentry>" << std::endl;

}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::failure( CmdLineInterfaceType& _cmd,
				    ArgException& e ) 
{ 
	static_cast<void>(_cmd); // unused
	std::cout << e.what() << std::endl;
	throw ExitException(1);
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::substituteSpecialChars( StringType& s,
				                                   CharType r,
												   StringType& x )
{
	size_t p;
	while ( (p = s.find_first_of(r)) != StringType::npos )
	{
		s.erase(p,1);
		s.insert(p,x);
	}
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::removeChar( StringType& s, CharType r)
{
	size_t p;
	while ( (p = s.find_first_of(r)) != StringType::npos )
	{
		s.erase(p,1);
	}
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::basename( StringType& s )
{
	size_t p = s.find_last_of('/');
	if ( p != StringType::npos )
	{
		s.erase(0, p + 1);
	}
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::printShortArg(ArgType* a)
{
	StringType lt = "&lt;"; 
	StringType gt = "&gt;"; 

	StringType id = a->shortID();
	substituteSpecialChars(id,'<',lt);
	substituteSpecialChars(id,'>',gt);
	removeChar(id,'[');
	removeChar(id,']');
	
	StringType choice = "opt";
	if ( a->isRequired() )
		choice = "plain";

	std::cout << "<arg choice='" << choice << '\'';
	if ( a->acceptsMultipleValues() )
		std::cout << " rep='repeat'";


	std::cout << '>';
	if ( !a->getFlag().empty() )
		std::cout << a->flagStartChar() << a->getFlag();
	else
		std::cout << a->nameStartString() << a->getName();
	if ( a->isValueRequired() )
	{
		StringType arg = a->shortID();
		removeChar(arg,'[');
		removeChar(arg,']');
		removeChar(arg,'<');
		removeChar(arg,'>');
		removeChar(arg,'.');
		arg.erase(0, arg.find_last_of(theDelimiter) + 1);
		std::cout << theDelimiter;
		std::cout << "<replaceable>" << arg << "</replaceable>";
	}
	std::cout << "</arg>" << std::endl;

}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::printLongArg(ArgType* a)
{
	StringType lt = "&lt;"; 
	StringType gt = "&gt;"; 

	StringType desc = a->getDescription();
	substituteSpecialChars(desc,'<',lt);
	substituteSpecialChars(desc,'>',gt);

	std::cout << "<varlistentry>" << std::endl;

	if ( !a->getFlag().empty() )
	{
		std::cout << "<term>" << std::endl;
		std::cout << "<option>";
		std::cout << a->flagStartChar() << a->getFlag();
		std::cout << "</option>" << std::endl;
		std::cout << "</term>" << std::endl;
	}

	std::cout << "<term>" << std::endl;
	std::cout << "<option>";
	std::cout << a->nameStartString() << a->getName();
	if ( a->isValueRequired() )
	{
		StringType arg = a->shortID();
		removeChar(arg,'[');
		removeChar(arg,']');
		removeChar(arg,'<');
		removeChar(arg,'>');
		removeChar(arg,'.');
		arg.erase(0, arg.find_last_of(theDelimiter) + 1);
		std::cout << theDelimiter;
		std::cout << "<replaceable>" << arg << "</replaceable>";
	}
	std::cout << "</option>" << std::endl;
	std::cout << "</term>" << std::endl;

	std::cout << "<listitem>" << std::endl;
	std::cout << "<para>" << std::endl;
	std::cout << desc << std::endl;
	std::cout << "</para>" << std::endl;
	std::cout << "</listitem>" << std::endl;

	std::cout << "</varlistentry>" << std::endl;
}

} //namespace TCLAP
#endif 
