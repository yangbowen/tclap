// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-


/******************************************************************************
 *
 *  file:  IgnoreRestVisitor.h
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


#ifndef TCLAP_IGNORE_REST_VISITOR_H
#define TCLAP_IGNORE_REST_VISITOR_H

#include <tclap/UseAllocatorBase.h>
#include <tclap/Visitor.h>
#include <tclap/Arg.h>

namespace TCLAP {

	class Visitor;
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class Arg;
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class IgnoreRestVisitor;

	/**
	 * A Visitor that tells the CmdLine to begin ignoring arguments after
	 * this one is parsed.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class IgnoreRestVisitor : public UseAllocatorBase<T_Alloc>, public Visitor {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using ArgType = Arg<T_Char, T_CharTraits, T_Alloc>;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		/**
		 * Constructor.
		 */
		IgnoreRestVisitor(const AllocatorType& alloc = AllocatorType()) : UseAllocatorBase<T_Alloc>(alloc), Visitor() {}

		/**
		 * Sets Arg::_ignoreRest.
		 */
		virtual void visit() override { ArgType::beginIgnoring(); }
	};

}

#endif
