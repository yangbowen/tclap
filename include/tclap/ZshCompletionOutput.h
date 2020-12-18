// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/****************************************************************************** 
 * 
 *  file:  ZshCompletionOutput.h
 * 
 *  Copyright (c) 2006, Oliver Kiddle
 *  Copyright (c) 2017 Google Inc.
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

#ifndef TCLAP_ZSHCOMPLETIONOUTPUT_H
#define TCLAP_ZSHCOMPLETIONOUTPUT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iostream>
#include <map>

#include <tclap/StringConvert.h>
#include <tclap/CmdLineInterface.h>
#include <tclap/CmdLineOutput.h>
#include <tclap/XorHandler.h>
#include <tclap/Arg.h>

namespace TCLAP {
	
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class ZshCompletionOutput;

/**
 * A class that generates a Zsh completion function as output from the usage()
 * method for the given CmdLine and its Args.
 */
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class ZshCompletionOutput : public CmdLineOutput<T_Char, T_CharTraits, T_Alloc>
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

		explicit ZshCompletionOutput(const AllocatorType& alloc = AllocatorType());

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

	protected:

		void basename( StringType& s );
		void quoteSpecialChars( StringType& s );

		StringType getMutexList(CmdLineInterfaceType& _cmd, ArgType* a );
		void printOption( ArgType* it, StringType mutex );
		void printArg( ArgType* it );

		std::map<StringType, StringType> common;
		CharType theDelimiter;
};

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::ZshCompletionOutput(const AllocatorType& alloc)
: CmdLineOutput<T_Char, T_CharTraits, T_Alloc>(alloc),
  common(std::map<StringType, StringType>()),
  theDelimiter(StringConvertType::fromConstBasicChar('='))
{
	common[StringConvertType::fromConstBasicCharString("host")] = StringConvertType::fromConstBasicCharString("_hosts");
	common[StringConvertType::fromConstBasicCharString("hostname")] = StringConvertType::fromConstBasicCharString("_hosts");
	common[StringConvertType::fromConstBasicCharString("file")] = StringConvertType::fromConstBasicCharString("_files");
	common[StringConvertType::fromConstBasicCharString("filename")] = StringConvertType::fromConstBasicCharString("_files");
	common[StringConvertType::fromConstBasicCharString("user")] = StringConvertType::fromConstBasicCharString("_users");
	common[StringConvertType::fromConstBasicCharString("username")] = StringConvertType::fromConstBasicCharString("_users");
	common[StringConvertType::fromConstBasicCharString("directory")] = StringConvertType::fromConstBasicCharString("_directories");
	common[StringConvertType::fromConstBasicCharString("path")] = StringConvertType::fromConstBasicCharString("_directories");
	common[StringConvertType::fromConstBasicCharString("url")] = StringConvertType::fromConstBasicCharString("_urls");
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::version(CmdLineInterfaceType& _cmd)
{
	std::cout << _cmd.getVersion() << std::endl;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::usage(CmdLineInterfaceType& _cmd )
{
	ArgListType argList = _cmd.getArgList();
	StringType progName = _cmd.getProgramName();
	StringType xversion = _cmd.getVersion();
	theDelimiter = _cmd.getDelimiter();
	basename(progName);

	std::cout << "#compdef " << progName << std::endl << std::endl <<
		"# " << progName << " version " << _cmd.getVersion() << std::endl << std::endl <<
		"_arguments -s -S";

	for (ArgListIteratorType it = argList.begin(); it != argList.end(); it++)
	{
		if ( (*it)->shortID().at(0) == '<' )
			printArg((*it));
		else if ( (*it)->getFlag() != "-" )
			printOption((*it), getMutexList(_cmd, *it));
	}

	std::cout << std::endl;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::failure( CmdLineInterfaceType& _cmd, ArgException<T_Char, T_CharTraits, T_Alloc>& e )
{
	static_cast<void>(_cmd); // unused
	std::cout << e.what() << std::endl;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::quoteSpecialChars( StringType& s )
{
	size_t idx = s.find_last_of(':');
	while ( idx != StringType::npos )
	{
		s.insert(idx, 1, '\\');
		idx = s.find_last_of(':', idx);
	}
	idx = s.find_last_of('\'');
	while ( idx != StringType::npos )
	{
		s.insert(idx, "'\\'");
		if (idx == 0)
			idx = StringType::npos;
		else
			idx = s.find_last_of('\'', --idx);
	}
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::basename( StringType& s )
{
	size_t p = s.find_last_of('/');
	if ( p != StringType::npos )
	{
		s.erase(0, p + 1);
	}
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::printArg(ArgType* a)
{
	static int count = 1;

	std::cout << " \\" << std::endl << "  '";
	if ( a->acceptsMultipleValues() )
		std::cout << '*';
	else
		std::cout << count++;
	std::cout << ':';
	if ( !a->isRequired() )
		std::cout << ':';

	std::cout << a->getName() << ':';
	typename std::map<StringType, StringType>::iterator compArg = common.find(a->getName());
	if ( compArg != common.end() )
	{
		std::cout << compArg->second;
	}
	else
	{
		std::cout << "_guard \"^-*\" " << a->getName();
	}
	std::cout << '\'';
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::printOption(ArgType* a, StringType mutex)
{
	StringType flag = a->flagStartChar() + a->getFlag();
	StringType name = a->nameStartString() + a->getName();
	StringType desc = a->getDescription();

	// remove full stop and capitalization from description as
	// this is the convention for zsh function
	if (!desc.compare(0, 12, "(required)  "))
	{
		desc.erase(0, 12);
	}
	if (!desc.compare(0, 15, "(OR required)  "))
	{
		desc.erase(0, 15);
	}
	size_t len = desc.length();
	if (len && desc.at(--len) == '.')
	{
		desc.erase(len);
	}
	if (len)
	{
		desc.replace(0, 1, 1, tolower(desc.at(0)));
	}

	std::cout << " \\" << std::endl << "  '" << mutex;

	if ( a->getFlag().empty() )
	{
		std::cout << name;
	}
	else
	{
		std::cout << "'{" << flag << ',' << name << "}'";
	}
	if ( theDelimiter == '=' && a->isValueRequired() )
		std::cout << "=-";
	quoteSpecialChars(desc);
	std::cout << '[' << desc << ']';

	if ( a->isValueRequired() )
	{
		StringType arg = a->shortID();
        // Example arg: "[-A <integer>] ..."
        size_t pos = arg.rfind(" ...");

        if (pos != StringType::npos) {
            arg.erase(pos);
        }

		arg.erase(0, arg.find_last_of(theDelimiter) + 1);
		if ( arg.at(arg.length()-1) == ']' )
			arg.erase(arg.length()-1);
		if ( arg.at(arg.length()-1) == ']' )
		{
			arg.erase(arg.length()-1);
		}
		if ( arg.at(0) == '<' )
		{
			arg.erase(arg.length()-1);
			arg.erase(0, 1);
		}
		size_t p = arg.find('|');
		if ( p != StringType::npos )
		{
			do
			{
				arg.replace(p, 1, 1, ' ');
			}
			while ( (p = arg.find_first_of('|', p)) != StringType::npos );
			quoteSpecialChars(arg);
			std::cout << ": :(" << arg << ')';
		}
		else
		{
			std::cout << ':' << arg;
			typename std::map<StringType, StringType>::iterator compArg = common.find(arg);
			if ( compArg != common.end() )
			{
				std::cout << ':' << compArg->second;
			}
		}
	}

	std::cout << '\'';
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto ZshCompletionOutput<T_Char, T_CharTraits, T_Alloc>::getMutexList( CmdLineInterfaceType& _cmd, ArgType* a) -> StringType
{
	XorHandler xorHandler = _cmd.getXorHandler();
	ArgVectorVectorType xorList = xorHandler.getXorList();
	
	if (a->getName() == "help" || a->getName() == "version")
	{
		return "(-)";
	}

	std::basic_ostringstream<T_Char, T_CharTraits, T_Alloc> list;
	if ( a->acceptsMultipleValues() )
	{
		list << '*';
	}

	for ( int i = 0; static_cast<unsigned int>(i) < xorList.size(); i++ )
	{
		for ( ArgVectorIteratorType it = xorList[i].begin();
			it != xorList[i].end();
			it++)
		if ( a == (*it) )
		{
			list << '(';
			for ( ArgVectorIteratorType iu = xorList[i].begin();
				iu != xorList[i].end();
				iu++ )
			{
				bool notCur = (*iu) != a;
				bool hasFlag = !(*iu)->getFlag().empty();
				if ( iu != xorList[i].begin() && (notCur || hasFlag) )
					list << ' ';
				if (hasFlag)
					list << (*iu)->flagStartChar() << (*iu)->getFlag() << ' ';
				if ( notCur || hasFlag )
					list << (*iu)->nameStartString() << (*iu)->getName();
			}
			list << ')';
			return list.str();
		}
	}
	
	// wasn't found in xor list
	if (!a->getFlag().empty()) {
		list << "(" << a->flagStartChar() << a->getFlag() << ' ' <<
			a->nameStartString() << a->getName() << ')';
	}
	
	return list.str();
}

} //namespace TCLAP
#endif
