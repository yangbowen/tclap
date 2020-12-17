// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-


/****************************************************************************** 
 * 
 *  file:  CmdLineInterface.h
 * 
 *  Copyright (c) 2003, Michael E. Smoot .
 *  Copyright (c) 2004, Michael E. Smoot, Daniel Aarno.
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

#ifndef TCLAP_COMMANDLINE_INTERFACE_H
#define TCLAP_COMMANDLINE_INTERFACE_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

#include <tclap/UseAllocatorBase.h>

namespace TCLAP {

class Visitor;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class Arg;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class CmdLineInterface;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class CmdLineOutput;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class XorHandler;

/**
 * The base class that manages the command line definition and passes
 * along the parsing to the appropriate Arg classes.
 */
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class CmdLineInterface : public UseAllocatorBase<T_Alloc>
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
		using ArgListIteratorType = typename ArgListType::const_iterator;
		using ArgVectorIteratorType = typename ArgVectorType::const_iterator;
		using CmdLineInterfaceType = CmdLineInterface<T_Char, T_CharTraits, T_Alloc>;
		using CmdLineOutputType = CmdLineOutput<T_Char, T_CharTraits, T_Alloc>;
		using XorHandlerType = XorHandler<T_Char, T_CharTraits, T_Alloc>;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		/**
		 * Destructor
		 */
		virtual ~CmdLineInterface() {}

		explicit CmdLineInterface(const AllocatorType& alloc) noexcept : UseAllocatorBase<T_Alloc>(alloc) {}

		/**
		 * Adds an argument to the list of arguments to be parsed.
		 * \param a - Argument to be added. 
		 */
		virtual void add( ArgType& a )=0;

		/**
		 * An alternative add.  Functionally identical.
		 * \param a - Argument to be added. 
		 */
		virtual void add( ArgType* a )=0;

		/**
		 * Add two Args that will be xor'd.  
		 * If this method is used, add does
		 * not need to be called.
		 * \param a - Argument to be added and xor'd. 
		 * \param b - Argument to be added and xor'd. 
		 */
		virtual void xorAdd( ArgType& a, ArgType& b )=0;

		/**
		 * Add a list of Args that will be xor'd.  If this method is used, 
		 * add does not need to be called.
		 * \param xors - List of Args to be added and xor'd. 
		 */
		virtual void xorAdd( const ArgVectorType& xors )=0;

		/**
		 * Parses the command line.
		 * \param argc - Number of arguments.
		 * \param argv - Array of arguments.
		 */
		virtual void parse(int argc, const CharType * const * argv)=0;

        /**
         * Parses the command line.
         * \param args - A vector of strings representing the args. 
         * args[0] is still the program name.
         */
        void parse(StringVectorType& args);

		/**
		 * Returns the CmdLineOutput object.
		 */
		virtual CmdLineOutputType* getOutput()=0;

		/**
		 * \param co - CmdLineOutput object that we want to use instead. 
		 */
		virtual void setOutput(CmdLineOutputType* co)=0;

		/**
		 * Returns the version string.
		 */
		virtual StringType& getVersion()=0;

		/**
		 * Returns the program name string.
		 */
		virtual StringType& getProgramName()=0;

		/**
		 * Returns the argList. 
		 */
		virtual ArgListType& getArgList()=0;

		/**
		 * Returns the XorHandler. 
		 */
		virtual XorHandlerType& getXorHandler()=0;

		/**
		 * Returns the delimiter string.
		 */
		virtual CharType getDelimiter()=0;

		/**
		 * Returns the message string.
		 */
		virtual StringType& getMessage()=0;

		/**
		 * Indicates whether or not the help and version switches were created
		 * automatically.
		 */
		virtual bool hasHelpAndVersion()=0;

		/** 
		 * Resets the instance as if it had just been constructed so that the
		 * instance can be reused. 
		 */
		virtual void reset()=0;
};

} //namespace


#endif 
