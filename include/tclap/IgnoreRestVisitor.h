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

#include <tclap/Visitor.h>
#include <tclap/Arg.h>

namespace TCLAP {

class Visitor;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class Arg;
template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
class IgnoreRestVisitor;

/**
 * A Visitor that tells the CmdLine to begin ignoring arguments after
 * this one is parsed.
 */
template<class T, typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class IgnoreRestVisitor : public Visitor
{
	public:
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using AllocatorType = T_Alloc;
		using ArgType = Arg<T_Char, T_CharTraits, T_Alloc>;

		/**
		 * Constructor.
		 */
		IgnoreRestVisitor() : Visitor() {}

		/**
		 * Sets Arg::_ignoreRest.
		 */
		void visit() { ArgType::beginIgnoring();  }
};

}

#endif
