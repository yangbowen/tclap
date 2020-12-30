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

#include <cassert>
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
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::StringViewType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::StringType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::StringVectorType;
		using typename CmdLineOutput<T_Char, T_CharTraits, T_Alloc>::OstreamType;
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

		explicit StdOutput(OstreamType* _os = nullptr, const AllocatorType& alloc = AllocatorType()) noexcept;

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
		OstreamType* os = nullptr;

		std::unique_ptr<ConvertedStdioStreams<T_Char, T_CharTraits>> convertedStdioStreams;

		/**
		 * Writes a brief usage message with short args.
		 * \param c - The CmdLine object the output is generated for.
		 */
		void _shortUsage(CmdLineInterfaceType& c) const;

		/**
		 * Writes a longer usage message with long and short args,
		 * provides descriptions and prints message.
		 * \param c - The CmdLine object the output is generated for.
		 */
		void _longUsage(CmdLineInterfaceType& c) const;

		/**
		 * This function inserts line breaks and indents long strings
		 * according to the params input. It will only break lines at spaces,
		 * commas and pipes.
		 * \param s - The string to be printed.
		 * \param maxWidth - The maxWidth allowed for the output line.
		 * \param indentSpaces - The number of spaces to indent the first line.
		 * \param secondLineOffset - The number of spaces to indent the second
		 * and all subsequent lines in addition to indentSpaces.
		 */
		void spacePrint(
			const StringType& s,
			std::size_t maxWidth,
			std::size_t indentSpaces,
			std::size_t secondLineOffset
		) const;
	};

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	StdOutput<T_Char, T_CharTraits, T_Alloc>::StdOutput(OstreamType* _os, const AllocatorType& alloc) noexcept
		: CmdLineOutput<T_Char, T_CharTraits, T_Alloc>(alloc),
		os(_os) {
		if (!os) {
			convertedStdioStreams = std::make_unique<ConvertedStdioStreams<T_Char, T_CharTraits>>();
			os = &convertedStdioStreams->getCerr();
		}
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::version(CmdLineInterfaceType& _cmd) {
		assert(os);
		*os << _cmd.getVersion() << std::endl;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::usage(CmdLineInterfaceType& _cmd) {
		assert(os);
		*os << std::endl << StringConvertType::fromConstBasicCharString("USAGE: ") << std::endl << std::endl;
		_shortUsage(_cmd);
		*os << std::endl << std::endl << StringConvertType::fromConstBasicCharString("Where: ") << std::endl << std::endl;
		_longUsage(_cmd);
		*os << std::endl;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::failure(CmdLineInterfaceType& _cmd, ArgException<T_Char, T_CharTraits, T_Alloc>& e) {
		assert(os);
		*os
			<< StringConvertType::fromConstBasicCharString("PARSE ERROR: ") << e.argId() << std::endl
			<< StringConvertType::fromConstBasicCharString("             ") << e.error() << std::endl << std::endl;
		if (_cmd.hasHelpAndVersion()) {
			*os << StringConvertType::fromConstBasicCharString("Brief USAGE: ") << std::endl;
			_shortUsage(_cmd);
			*os << std::endl << StringConvertType::fromConstBasicCharString("For complete USAGE and HELP type: ")
				<< std::endl << StringConvertType::fromConstBasicCharString("   ") << _cmd.getProgramName() << StringConvertType::fromConstBasicCharString(" ")
				<< ArgType::nameStartString() << "help"
				<< std::endl << std::endl;
		} else
			usage(_cmd);
		throw ExitException(1);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::_shortUsage(CmdLineInterfaceType& _cmd) const {
		ArgListType argList = _cmd.getArgList();
		StringType progName = _cmd.getProgramName();
		XorHandlerType xorHandler = _cmd.getXorHandler();
		ArgVectorVectorType xorList = xorHandler.getXorList();

		StringType s = progName + StringConvertType::fromConstBasicCharString(" ");

		// first the xor
		for (const ArgVectorType& xorEntry : xorList) {
			s += StringConvertType::fromConstBasicCharString(" {");
			for (const ArgType* const& arg : xorEntry)
				s += arg->shortID() + StringConvertType::fromConstBasicCharString("|");
			s.back() = StringConvertType::fromConstBasicChar('}');
		}

		// then the rest
		for (const ArgType* const& arg : argList)
			if (!xorHandler.contains(arg))
				s += StringConvertType::fromConstBasicCharString(" ") + arg->shortID();

		// if the program name is too long, then adjust the second line offset 
		std::size_t secondLineOffset = progName.size() + 2;
		if (secondLineOffset > 75 / 2)
			secondLineOffset = 75 / 2;

		spacePrint(s, 75, 3, secondLineOffset);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::_longUsage(CmdLineInterfaceType& _cmd) const {
		assert(os);
		ArgListType argList = _cmd.getArgList();
		StringType message = _cmd.getMessage();
		XorHandlerType xorHandler = _cmd.getXorHandler();
		ArgVectorVectorType xorList = xorHandler.getXorList();

		// first the xor 
		for (const ArgVectorType& xorEntry : xorList) {
			bool is_first_arg = true;
			for (const ArgType* const& arg : xorEntry) {
				if (is_first_arg)
					is_first_arg = false;
				else
					spacePrint(StringConvertType::fromConstBasicCharString("-- OR --"), 75, 9, 0);
				spacePrint(arg->longID(), 75, 3, 3);
				spacePrint(arg->getDescription(), 75, 5, 0);
			}
			*os << std::endl << std::endl;
		}

		// then the rest
		for (const ArgType* const& arg : argList) if (!xorHandler.contains(arg)) {
			spacePrint(arg->longID(), 75, 3, 3);
			spacePrint(arg->getDescription(), 75, 5, 0);
			*os << std::endl;
		}

		*os << std::endl;

		spacePrint(message, 75, 3, 0);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void StdOutput<T_Char, T_CharTraits, T_Alloc>::spacePrint(
		const StringType& s,
		std::size_t maxWidth,
		std::size_t indentSpaces,
		std::size_t secondLineOffset
	) const {
		assert(os);
		StringViewType strview_remaining = s;

		assert(strview_remaining.size() <= std::numeric_limits<std::size_t>::max() - indentSpaces);
		if (maxWidth > 0 && strview_remaining.size() + indentSpaces > maxWidth) {
			bool is_first_line = true;
			assert(maxWidth > indentSpaces);
			std::size_t allowedLen = maxWidth - indentSpaces;// allowedLen > 0			
			while (!strview_remaining.empty()) {
				// find the substring length
				std::size_t stringLen = std::min(strview_remaining.size(), allowedLen);// stringLen > 0

				// trim the length so it doesn't end in middle of a word
				if (stringLen < strview_remaining.size()) {
					static const std::array arr_wordbreak = StringConvertType::fromConstBasicCharArray(" ,|");
					StringViewType strview_wordbreak(arr_wordbreak.cbegin(), arr_wordbreak.cend());
					std::size_t result_find = strview_remaining.substr(0, stringLen).find_last_of(strview_wordbreak);
					// otherwise, the word is longer than the line, so just split wherever the line ends
					if (result_find != StringViewType::npos) stringLen = result_find + 1;
				}

				// check for newlines
				{
					std::size_t result_find = strview_remaining.substr(0, stringLen).find_first_of(StringConvertType::fromConstBasicChar('\n'));
					if (result_find != StringViewType::npos) stringLen = result_find + 1;
				}

				// print the indent
				*os << StringType(indentSpaces, StringConvertType::fromConstBasicChar(' '));

				if (is_first_line) {
					assert(allowedLen > secondLineOffset);
					// handle second line offsets
					indentSpaces += secondLineOffset;
					// adjust allowed len
					allowedLen -= secondLineOffset;// allowedLen > 0
					is_first_line = false;
				}

				*os << strview_remaining.substr(0, stringLen) << std::endl;

				strview_remaining.remove_prefix(stringLen);
				// so we don't start a line with a space
				{
					std::size_t result_find = strview_remaining.find_first_not_of(StringConvertType::fromConstBasicChar(' '));
					if (result_find == StringViewType::npos) {
						result_find = strview_remaining.size();
					}
					strview_remaining.remove_prefix(result_find);
				}
			}
		} else {
			*os << StringType(indentSpaces, StringConvertType::fromConstBasicChar(' '));
			*os << strview_remaining << std::endl;
		}
	}

} //namespace TCLAP
#endif 
