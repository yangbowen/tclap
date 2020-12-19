// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  StdOutput.h
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

#ifndef TCLAP_STDCMDLINEOUTPUT_H
#define TCLAP_STDCMDLINEOUTPUT_H

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

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class StdOutput;

	/**
	 * A class that isolates any output from the CmdLine object so that it
	 * may be easily modified.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class StdOutput : public CmdLineOutput<T_Char, T_CharTraits, T_Alloc> {
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
		using OstreamType = std::basic_ostream<T_Char, T_CharTraits>;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		explicit StdOutput(const AllocatorType& alloc = AllocatorType()) noexcept : CmdLineOutput<T_Char, T_CharTraits, T_Alloc>(alloc) {}

		/**
		 * Prints the usage to stdout.  Can be overridden to
		 * produce alternative behavior.
		 * \param c - The CmdLine object the output is generated for.
		 */
		virtual void usage(CmdLineInterfaceType& c) override;

		/**
		 * Prints the version to stdout. Can be overridden
		 * to produce alternative behavior.
		 * \param c - The CmdLine object the output is generated for.
		 */
		virtual void version(CmdLineInterfaceType& c) override;

		/**
		 * Prints (to stderr) an error message, short usage
		 * Can be overridden to produce alternative behavior.
		 * \param c - The CmdLine object the output is generated for.
		 * \param e - The ArgException that caused the failure.
		 */
		virtual void failure(CmdLineInterfaceType& c, ArgException<T_Char, T_CharTraits, T_Alloc>& e) override;

	protected:

		/**
		 * Writes a brief usage message with short args.
		 * \param c - The CmdLine object the output is generated for.
		 * \param os - The stream to write the message to.
		 */
		void _shortUsage(CmdLineInterfaceType& c, OstreamType& os) const;

		/**
		 * Writes a longer usage message with long and short args,
		 * provides descriptions and prints message.
		 * \param c - The CmdLine object the output is generated for.
		 * \param os - The stream to write the message to.
		 */
		void _longUsage(CmdLineInterfaceType& c, OstreamType& os) const;

		/**
		 * This function inserts line breaks and indents long strings
		 * according the  params input. It will only break lines at spaces,
		 * commas and pipes.
		 * \param os - The stream to be printed to.
		 * \param s - The string to be printed.
		 * \param maxWidth - The maxWidth allowed for the output line.
		 * \param indentSpaces - The number of spaces to indent the first line.
		 * \param secondLineOffset - The number of spaces to indent the second
		 * and all subsequent lines in addition to indentSpaces.
		 */
		void spacePrint(OstreamType& os,
			const StringType& s,
			int maxWidth,
			int indentSpaces,
			int secondLineOffset) const;

	};


	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::version(CmdLineInterfaceType& _cmd) {
		StringType progName = _cmd.getProgramName();
		StringType xversion = _cmd.getVersion();

		std::cout << std::endl << progName << StringConvertType::fromConstBasicCharString("  version: ")
			<< xversion << std::endl << std::endl;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::usage(CmdLineInterfaceType& _cmd) {
		std::cout << std::endl << StringConvertType::fromConstBasicCharString("USAGE: ") << std::endl << std::endl;

		_shortUsage(_cmd, std::cout);

		std::cout << std::endl << std::endl << StringConvertType::fromConstBasicCharString("Where: ") << std::endl << std::endl;

		_longUsage(_cmd, std::cout);

		std::cout << std::endl;

	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::failure(CmdLineInterfaceType& _cmd, ArgException<T_Char, T_CharTraits, T_Alloc>& e) {
		StringType progName = _cmd.getProgramName();

		std::cerr << StringConvertType::fromConstBasicCharString("PARSE ERROR: ") << e.argId() << std::endl
			<< StringConvertType::fromConstBasicCharString("             ") << e.error() << std::endl << std::endl;

		if (_cmd.hasHelpAndVersion()) 		{
			std::cerr << StringConvertType::fromConstBasicCharString("Brief USAGE: ") << std::endl;

			_shortUsage(_cmd, std::cerr);

			std::cerr << std::endl << StringConvertType::fromConstBasicCharString("For complete USAGE and HELP type: ")
				<< std::endl << StringConvertType::fromConstBasicCharString("   ") << progName << StringConvertType::fromConstBasicCharString(" ")
				<< ArgType::nameStartString() << "help"
				<< std::endl << std::endl;
		} 	else
			usage(_cmd);

		throw ExitException(1);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void
		StdOutput<T_Char, T_CharTraits, T_Alloc>::_shortUsage(CmdLineInterfaceType& _cmd,
			OstreamType& os) const {
		ArgListType argList = _cmd.getArgList();
		StringType progName = _cmd.getProgramName();
		XorHandlerType xorHandler = _cmd.getXorHandler();
		ArgVectorVectorType xorList = xorHandler.getXorList();

		StringType s = progName + StringConvertType::fromConstBasicCharString(" ");

		// first the xor
		for (const ArgVectorType& xorEntry : xorList) 		{
			s += StringConvertType::fromConstBasicCharString(" {");
			for (const ArgType* const& arg : xorEntry)
				s += arg->shortID() + StringConvertType::fromConstBasicCharString("|");

			s[s.length() - 1] = StringConvertType::fromConstBasicChar('}');
		}

		// then the rest
		for (const ArgType* const& arg : argList)
			if (!xorHandler.contains(arg))
				s += StringConvertType::fromConstBasicCharString(" ") + arg->shortID();

		// if the program name is too long, then adjust the second line offset 
		int secondLineOffset = static_cast<int>(progName.length()) + 2;
		if (secondLineOffset > 75 / 2)
			secondLineOffset = static_cast<int>(75 / 2);

		spacePrint(os, s, 75, 3, secondLineOffset);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void
		StdOutput<T_Char, T_CharTraits, T_Alloc>::_longUsage(CmdLineInterfaceType& _cmd,
			OstreamType& os) const {
		ArgListType argList = _cmd.getArgList();
		StringType message = _cmd.getMessage();
		XorHandlerType xorHandler = _cmd.getXorHandler();
		ArgVectorVectorType xorList = xorHandler.getXorList();

		// first the xor 
		for (int i = 0; static_cast<unsigned int>(i) < xorList.size(); i++) 		{
			for (ArgVectorIteratorType it = xorList[i].begin();
				it != xorList[i].end();
				it++) 				{
				spacePrint(os, (*it)->longID(), 75, 3, 3);
				spacePrint(os, (*it)->getDescription(), 75, 5, 0);

				if (it + 1 != xorList[i].end())
					spacePrint(os, StringConvertType::fromConstBasicCharString("-- OR --"), 75, 9, 0);
			}
			os << std::endl << std::endl;
		}

		// then the rest
		for (ArgListIteratorType it = argList.begin(); it != argList.end(); it++)
			if (!xorHandler.contains((*it))) 			{
				spacePrint(os, (*it)->longID(), 75, 3, 3);
				spacePrint(os, (*it)->getDescription(), 75, 5, 0);
				os << std::endl;
			}

		os << std::endl;

		spacePrint(os, message, 75, 3, 0);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::spacePrint(OstreamType& os,
		const StringType& s,
		int maxWidth,
		int indentSpaces,
		int secondLineOffset) const {
		int len = static_cast<int>(s.length());

		if ((len + indentSpaces > maxWidth) && maxWidth > 0) 		{
			int allowedLen = maxWidth - indentSpaces;
			int start = 0;
			while (start < len) 				{
				// find the substring length
				// int stringLen = std::min<int>( len - start, allowedLen );
				// doing it this way to support a VisualC++ 2005 bug 
				using namespace std;
				int stringLen = min<int>(len - start, allowedLen);

				// trim the length so it doesn't end in middle of a word
				if (stringLen == allowedLen)
					while (stringLen >= 0 &&
						s[stringLen + start] != StringConvertType::fromConstBasicChar(' ') &&
						s[stringLen + start] != StringConvertType::fromConstBasicChar(',') &&
						s[stringLen + start] != StringConvertType::fromConstBasicChar('|'))
						stringLen--;

				// ok, the word is longer than the line, so just split 
				// wherever the line ends
				if (stringLen <= 0)
					stringLen = allowedLen;

				// check for newlines
				for (int i = 0; i < stringLen; i++)
					if (s[start + i] == StringConvertType::fromConstBasicChar('\n'))
						stringLen = i + 1;

				// print the indent	
				for (int i = 0; i < indentSpaces; i++)
					os << StringConvertType::fromConstBasicCharString(" ");

				if (start == 0) 						{
					// handle second line offsets
					indentSpaces += secondLineOffset;

					// adjust allowed len
					allowedLen -= secondLineOffset;
				}

				os << s.substr(start, stringLen) << std::endl;

				// so we don't start a line with a space
				while (s[stringLen + start] == StringConvertType::fromConstBasicChar(' ') && start < len)
					start++;

				start += stringLen;
			}
		} 	else 		{
			for (int i = 0; i < indentSpaces; i++)
				os << StringConvertType::fromConstBasicCharString(" ");
			os << s << std::endl;
		}
	}

} //namespace TCLAP
#endif 
