// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-


/******************************************************************************
 *
 *  file:  HelpVisitor.h
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

#ifndef TCLAP_HELP_VISITOR_H
#define TCLAP_HELP_VISITOR_H

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
	class HelpVisitor;

	/**
	 * A Visitor object that calls the usage method of the given CmdLineOutput
	 * object for the specified CmdLine object.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class HelpVisitor : public UseAllocatorBase<T_Alloc>, public Visitor {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using CmdLineInterfaceType = CmdLineInterface<T_Char, T_CharTraits, T_Alloc>;
		using CmdLineOutputType = CmdLineOutput<T_Char, T_CharTraits, T_Alloc>;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		HelpVisitor(const HelpVisitor& rhs) = delete;
		HelpVisitor& operator=(const HelpVisitor& rhs) = delete;

		/**
		 * Constructor.
		 * \param cmd - The CmdLine the output will be generated for.
		 * \param out - The type of output.
		 */
		HelpVisitor(CmdLineInterfaceType* cmd, CmdLineOutputType** out, const AllocatorType& alloc = AllocatorType())
			: UseAllocatorBase<T_Alloc>(alloc), Visitor(), _cmd(cmd), _out(out) {}

		/**
		 * Calls the usage method of the CmdLineOutput for the
		 * specified CmdLine.
		 */
		virtual void visit() override { (*_out)->usage(*_cmd); throw ExitException(0); }

	protected:
		/**
		 * The CmdLine the output will be generated for.
		 */
		CmdLineInterfaceType* _cmd;

		/**
		 * The output object.
		 */
		CmdLineOutputType** _out;
	};

}

#endif
