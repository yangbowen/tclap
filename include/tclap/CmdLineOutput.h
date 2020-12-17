// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-



/****************************************************************************** 
 * 
 *  file:  CmdLineOutput.h
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

#ifndef TCLAP_CMDLINEOUTPUT_H
#define TCLAP_CMDLINEOUTPUT_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include <tclap/UseAllocatorBase.h>

namespace TCLAP {

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class Arg;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class CmdLineInterface;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class CmdLineOutput;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class XorHandler;
class ArgException;

/**
 * The interface that any output object must implement.
 */
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class CmdLineOutput : public UseAllocatorBase<T_Alloc>
{
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using StringType = std::basic_string<T_Char, T_CharTraits, T_Alloc>;
		using StringVectorType = std::vector<StringType, typename std::allocator_traits<AllocatorType>::template rebind_alloc<StringType>>;
		using ArgType = Arg<T_Char, T_CharTraits, T_Alloc>;
		using ArgListType = std::list<ArgType*, typename std::allocator_traits<AllocatorType>::template rebind_alloc<ArgType*>>;
		using ArgVectorType = std::vector<ArgType*, typename std::allocator_traits<AllocatorType>::template rebind_alloc<ArgType*>>;
		using ArgVectorVectorType = std::vector<ArgVectorType, typename std::allocator_traits<AllocatorType>::template rebind_alloc<ArgVectorType>>;
		using ArgListIteratorType = typename ArgListType::const_iterator;
		using ArgVectorIteratorType = typename ArgVectorType::const_iterator;
		using CmdLineInterfaceType = CmdLineInterface<T_Char, T_CharTraits, T_Alloc>;
		using CmdLineOutputType = CmdLineOutput<T_Char, T_CharTraits, T_Alloc>;
		using XorHandlerType = XorHandler<T_Char, T_CharTraits, T_Alloc>;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		/**
		 * Virtual destructor.
		 */
		virtual ~CmdLineOutput() {}

		explicit CmdLineOutput(const AllocatorType& alloc) noexcept : UseAllocatorBase<T_Alloc>(alloc) {}

		/**
		 * Generates some sort of output for the USAGE. 
		 * \param c - The CmdLine object the output is generated for. 
		 */
		virtual void usage(CmdLineInterfaceType& c)=0;

		/**
		 * Generates some sort of output for the version. 
		 * \param c - The CmdLine object the output is generated for. 
		 */
		virtual void version(CmdLineInterfaceType& c)=0;

		/**
		 * Generates some sort of output for a failure. 
		 * \param c - The CmdLine object the output is generated for. 
		 * \param e - The ArgException that caused the failure. 
		 */
		virtual void failure( CmdLineInterfaceType& c,
						      ArgException& e )=0;

};

} //namespace TCLAP
#endif 
