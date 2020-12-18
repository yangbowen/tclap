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

#include <tclap/StringConvert.h>
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
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::StringConvertType;
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
		virtual void failure(CmdLineInterfaceType& c, ArgException<T_Char, T_CharTraits, T_Alloc>& e );

	    DocBookOutput(const AllocatorType& alloc = AllocatorType()) : CmdLineOutput<T_Char, T_CharTraits, T_Alloc>(alloc), theDelimiter(StringConvertType::fromConstBasicChar('=')) {}
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

		void printShortArg(const ArgType* it);
		void printLongArg(const ArgType* it);

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

	std::cout << StringConvertType::fromConstBasicCharString("<?xml version=\"1.0\"?>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<!DOCTYPE refentry PUBLIC \"-//OASIS//DTD DocBook XML V4.2//EN\"") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("\t\"http://www.oasis-open.org/docbook/xml/4.2/docbookx.dtd\">") << std::endl << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<refentry>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<refmeta>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<refentrytitle>") << progName << StringConvertType::fromConstBasicCharString("</refentrytitle>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<manvolnum>1</manvolnum>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</refmeta>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<refnamediv>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<refname>") << progName << StringConvertType::fromConstBasicCharString("</refname>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<refpurpose>") << _cmd.getMessage() << StringConvertType::fromConstBasicCharString("</refpurpose>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</refnamediv>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<refsynopsisdiv>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<cmdsynopsis>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<command>") << progName << StringConvertType::fromConstBasicCharString("</command>") << std::endl;

	// xor
	for (const ArgVectorType& xorEntry : xorList) 	{
		std::cout << StringConvertType::fromConstBasicCharString("<group choice=\"req\">") << std::endl;
		for (const ArgType* const& arg : xorEntry) printShortArg(arg);
		std::cout << StringConvertType::fromConstBasicCharString("</group>") << std::endl;
	}
	// rest of args
	for (const ArgType* const& arg : argList) if (!xorHandler.contains(arg)) printShortArg(arg);

 	std::cout << StringConvertType::fromConstBasicCharString("</cmdsynopsis>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</refsynopsisdiv>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<refsect1>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<title>Description</title>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<para>") << std::endl;
	std::cout << _cmd.getMessage() << std::endl; 
	std::cout << StringConvertType::fromConstBasicCharString("</para>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</refsect1>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<refsect1>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<title>Options</title>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<variablelist>") << std::endl;	
	for (const ArgType* const& arg : argList) printLongArg(arg);
	std::cout << StringConvertType::fromConstBasicCharString("</variablelist>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</refsect1>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<refsect1>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<title>Version</title>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<para>") << std::endl;
	std::cout << xversion << std::endl; 
	std::cout << StringConvertType::fromConstBasicCharString("</para>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</refsect1>") << std::endl;
	
	std::cout << StringConvertType::fromConstBasicCharString("</refentry>") << std::endl;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::failure( CmdLineInterfaceType& _cmd, ArgException<T_Char, T_CharTraits, T_Alloc>& e )
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
	size_t p = s.find_last_of(StringConvertType::fromConstBasicChar('/'));
	if ( p != StringType::npos )
	{
		s.erase(0, p + 1);
	}
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::printShortArg(const ArgType* a)
{
	StringType lt = StringConvertType::fromConstBasicCharString("&lt;");
	StringType gt = StringConvertType::fromConstBasicCharString("&gt;");

	StringType id = a->shortID();
	substituteSpecialChars(id, StringConvertType::fromConstBasicChar('<'),lt);
	substituteSpecialChars(id, StringConvertType::fromConstBasicChar('>'),gt);
	removeChar(id, StringConvertType::fromConstBasicChar('['));
	removeChar(id, StringConvertType::fromConstBasicChar(']'));
	
	StringType choice = StringConvertType::fromConstBasicCharString("opt");
	if ( a->isRequired() ) choice = StringConvertType::fromConstBasicCharString("plain");

	std::cout << StringConvertType::fromConstBasicCharString("<arg choice=\"") << choice << StringConvertType::fromConstBasicChar('\"');
	if ( a->acceptsMultipleValues() ) std::cout << StringConvertType::fromConstBasicCharString(" rep=\"repeat\"");

	std::cout << StringConvertType::fromConstBasicChar('>');
	if ( !a->getFlag().empty() )
		std::cout << a->flagStartChar() << a->getFlag();
	else
		std::cout << a->nameStartString() << a->getName();
	if ( a->isValueRequired() )
	{
		StringType arg = a->shortID();
		removeChar(arg, StringConvertType::fromConstBasicChar('['));
		removeChar(arg, StringConvertType::fromConstBasicChar(']'));
		removeChar(arg, StringConvertType::fromConstBasicChar('<'));
		removeChar(arg, StringConvertType::fromConstBasicChar('>'));
		removeChar(arg, StringConvertType::fromConstBasicChar('.'));
		arg.erase(0, arg.find_last_of(theDelimiter) + 1);
		std::cout << theDelimiter;
		std::cout << StringConvertType::fromConstBasicCharString("<replaceable>") << arg << StringConvertType::fromConstBasicCharString("</replaceable>");
	}
	std::cout << StringConvertType::fromConstBasicCharString("</arg>") << std::endl;

}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void DocBookOutput<T_Char, T_CharTraits, T_Alloc>::printLongArg(const ArgType* a)
{
	StringType lt = StringConvertType::fromConstBasicCharString("&lt;");
	StringType gt = StringConvertType::fromConstBasicCharString("&gt;");

	StringType desc = a->getDescription();
	substituteSpecialChars(desc, StringConvertType::fromConstBasicChar('<'),lt);
	substituteSpecialChars(desc, StringConvertType::fromConstBasicChar('>'),gt);

	std::cout << StringConvertType::fromConstBasicCharString("<varlistentry>") << std::endl;

	if ( !a->getFlag().empty() )
	{
		std::cout << StringConvertType::fromConstBasicCharString("<term>") << std::endl;
		std::cout << StringConvertType::fromConstBasicCharString("<option>") << std::endl;
		std::cout << a->flagStartChar() << a->getFlag();
		std::cout << StringConvertType::fromConstBasicCharString("</option>") << std::endl;
		std::cout << StringConvertType::fromConstBasicCharString("</term>") << std::endl;
	}

	std::cout << StringConvertType::fromConstBasicCharString("<term>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<option>");
	std::cout << a->nameStartString() << a->getName();
	if ( a->isValueRequired() )
	{
		StringType arg = a->shortID();
		removeChar(arg, StringConvertType::fromConstBasicChar('['));
		removeChar(arg, StringConvertType::fromConstBasicChar(']'));
		removeChar(arg, StringConvertType::fromConstBasicChar('<'));
		removeChar(arg, StringConvertType::fromConstBasicChar('>'));
		removeChar(arg, StringConvertType::fromConstBasicChar('.'));
		arg.erase(0, arg.find_last_of(theDelimiter) + 1);
		std::cout << theDelimiter;
		std::cout << StringConvertType::fromConstBasicCharString("<replaceable>") << arg << StringConvertType::fromConstBasicCharString("</replaceable>");
	}
	std::cout << StringConvertType::fromConstBasicCharString("</option>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</term>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("<listitem>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("<para>") << std::endl;
	std::cout << desc << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</para>") << std::endl;
	std::cout << StringConvertType::fromConstBasicCharString("</listitem>") << std::endl;

	std::cout << StringConvertType::fromConstBasicCharString("</varlistentry>") << std::endl;
}

} //namespace TCLAP
#endif 
