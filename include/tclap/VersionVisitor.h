// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/****************************************************************************** 
 * 
 *  file:  VersionVisitor.h
 * 
 *  Copyright (c) 2003, Michael E. Smoot .
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


#ifndef TCLAP_VERSION_VISITOR_H
#define TCLAP_VERSION_VISITOR_H

#include <tclap/UseAllocatorBase.h>
#include <tclap/CmdLineInterface.h>
#include <tclap/CmdLineOutput.h>
#include <tclap/Visitor.h>

namespace TCLAP {

class Visitor;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class CmdLineInterface;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class CmdLineOutput;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class VersionVisitor;

/**
 * A Visitor that will call the version method of the given CmdLineOutput
 * for the specified CmdLine object and then exit.
 */
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class VersionVisitor : public UseAllocatorBase<T_Alloc>, public Visitor
{
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using CmdLineInterfaceType = CmdLineInterface<T_Char, T_CharTraits, T_Alloc>;
		using CmdLineOutputType = CmdLineOutput<T_Char, T_CharTraits, T_Alloc>;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		VersionVisitor(const VersionVisitor& rhs) = delete;
		VersionVisitor& operator=(const VersionVisitor& rhs) = delete;

		/**
		 * Constructor.
		 * \param cmd - The CmdLine the output is generated for. 
		 * \param out - The type of output. 
		 */
		VersionVisitor( CmdLineInterfaceType* cmd, CmdLineOutputType** out, const AllocatorType& alloc = AllocatorType() ) 
				: UseAllocatorBase<T_Alloc>(alloc), Visitor(), _cmd( cmd ), _out( out ) { }

		/**
		 * Calls the version method of the output object using the
		 * specified CmdLine.
		 */
		void visit() { 
		    (*_out)->version(*_cmd); 
		    throw ExitException(0); 
		}

	protected:
		/**
		 * The CmdLine of interest.
		 */
		CmdLineInterfaceType* _cmd;

		/**
		 * The output object. 
		 */
		CmdLineOutputType** _out;
};

}

#endif
