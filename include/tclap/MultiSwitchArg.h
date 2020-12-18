// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-


/****************************************************************************** 
*
*  file:  MultiSwitchArg.h
*
*  Copyright (c) 2003, Michael E. Smoot .
*  Copyright (c) 2004, Michael E. Smoot, Daniel Aarno.
*  Copyright (c) 2005, Michael E. Smoot, Daniel Aarno, Erik Zeek.
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


#ifndef TCLAP_MULTI_SWITCH_ARG_H
#define TCLAP_MULTI_SWITCH_ARG_H

#include <string>
#include <vector>

#include <tclap/SwitchArg.h>

namespace TCLAP {

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class MultiSwitchArg;

/**
* A multiple switch argument.  If the switch is set on the command line, then
* the getValue method will return the number of times the switch appears.
*/
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class MultiSwitchArg : public SwitchArg<T_Char, T_CharTraits, T_Alloc>
{
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::StringType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::StringVectorType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgListType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgVectorType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgListIteratorType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgVectorIteratorType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::CmdLineInterfaceType;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;
		using Arg<T_Char, T_CharTraits, T_Alloc>::addToList;
		using Arg<T_Char, T_CharTraits, T_Alloc>::beginIgnoring;
		using Arg<T_Char, T_CharTraits, T_Alloc>::ignoreRest;
		using Arg<T_Char, T_CharTraits, T_Alloc>::delimiter;
		using Arg<T_Char, T_CharTraits, T_Alloc>::blankChar;
		using Arg<T_Char, T_CharTraits, T_Alloc>::flagStartChar;
		using Arg<T_Char, T_CharTraits, T_Alloc>::flagStartString;
		using Arg<T_Char, T_CharTraits, T_Alloc>::nameStartString;
		using Arg<T_Char, T_CharTraits, T_Alloc>::ignoreNameString;
		using Arg<T_Char, T_CharTraits, T_Alloc>::setDelimiter;
		using Arg<T_Char, T_CharTraits, T_Alloc>::processArg;
		using Arg<T_Char, T_CharTraits, T_Alloc>::operator==;
		using Arg<T_Char, T_CharTraits, T_Alloc>::getFlag;
		using Arg<T_Char, T_CharTraits, T_Alloc>::getName;
		using Arg<T_Char, T_CharTraits, T_Alloc>::getDescription;
		using Arg<T_Char, T_CharTraits, T_Alloc>::isRequired;
		using Arg<T_Char, T_CharTraits, T_Alloc>::forceRequired;
		using Arg<T_Char, T_CharTraits, T_Alloc>::xorSet;
		using Arg<T_Char, T_CharTraits, T_Alloc>::isValueRequired;
		using Arg<T_Char, T_CharTraits, T_Alloc>::isSet;
		using Arg<T_Char, T_CharTraits, T_Alloc>::isIgnoreable;
		using Arg<T_Char, T_CharTraits, T_Alloc>::argMatches;
		using Arg<T_Char, T_CharTraits, T_Alloc>::toString;
		using Arg<T_Char, T_CharTraits, T_Alloc>::shortID;
		using Arg<T_Char, T_CharTraits, T_Alloc>::longID;
		using Arg<T_Char, T_CharTraits, T_Alloc>::trimFlag;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_hasBlanks;
		using Arg<T_Char, T_CharTraits, T_Alloc>::setRequireLabel;
		using Arg<T_Char, T_CharTraits, T_Alloc>::allowMore;
		using Arg<T_Char, T_CharTraits, T_Alloc>::acceptsMultipleValues;
		using Arg<T_Char, T_CharTraits, T_Alloc>::reset;
		using SwitchArg<T_Char, T_CharTraits, T_Alloc>::processArg;
		using SwitchArg<T_Char, T_CharTraits, T_Alloc>::combinedSwitchesMatch;
		using SwitchArg<T_Char, T_CharTraits, T_Alloc>::getValue;
		using SwitchArg<T_Char, T_CharTraits, T_Alloc>::operator bool;
		using SwitchArg<T_Char, T_CharTraits, T_Alloc>::reset;

		/**
		 * MultiSwitchArg constructor.
		 * \param flag - The one character flag that identifies this
		 * argument on the command line.
		 * \param name - A one word name for the argument.  Can be
		 * used as a long flag on the command line.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param init - Optional. The initial/default value of this Arg. 
		 * Defaults to 0.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		MultiSwitchArg(const StringType& flag, 
				const StringType& name,
				const StringType& desc,
				int init = 0,
				Visitor* v = nullptr,
				const AllocatorType& alloc = AllocatorType());


		/**
		 * MultiSwitchArg constructor.
		 * \param flag - The one character flag that identifies this
		 * argument on the command line.
		 * \param name - A one word name for the argument.  Can be
		 * used as a long flag on the command line.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param parser - A CmdLine parser object to add this Arg to
		 * \param init - Optional. The initial/default value of this Arg. 
		 * Defaults to 0.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		MultiSwitchArg(const StringType& flag, 
				const StringType& name,
				const StringType& desc,
				CmdLineInterfaceType& parser,
				int init = 0,
				Visitor* v = nullptr,
				const AllocatorType& alloc = AllocatorType());


		/**
		 * Handles the processing of the argument.
		 * This re-implements the SwitchArg version of this method to set the
		 * _value of the argument appropriately.
		 * \param i - Pointer the the current argument in the list.
		 * \param args - Mutable list of strings. Passed
		 * in from main().
		 */
		virtual bool processArg(int* i, StringVectorType& args); 

		/**
		 * Returns int, the number of times the switch has been set.
		 */
		int getValue() const { return _value; }

		/**
		 * Returns the shortID for this Arg.
		 */
		StringType shortID(const StringType& val) const;

		/**
		 * Returns the longID for this Arg.
		 */
		StringType longID(const StringType& val) const;
		
		void reset();

	protected:
		using Arg<T_Char, T_CharTraits, T_Alloc>::_flag;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_name;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_description;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_required;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_requireLabel;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_valueRequired;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_alreadySet;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_visitor;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_ignoreable;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_xorSet;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_acceptsMultipleValues;
		using Arg<T_Char, T_CharTraits, T_Alloc>::_checkWithVisitor;

		/**
		 * The value of the switch.
		 */
		int _value;

		/**
		 * Used to support the reset() method so that ValueArg can be
		 * reset to their constructed value.
		 */
		int _default;
};

//////////////////////////////////////////////////////////////////////
//BEGIN MultiSwitchArg.cpp
//////////////////////////////////////////////////////////////////////
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline MultiSwitchArg<T_Char, T_CharTraits, T_Alloc>::MultiSwitchArg(const StringType& flag,
					const StringType& name,
					const StringType& desc,
					int init,
					Visitor* v,
	                const AllocatorType& alloc )
: SwitchArg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, false, v, alloc),
_value( init ),
_default( init )
{ }

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline MultiSwitchArg<T_Char, T_CharTraits, T_Alloc>::MultiSwitchArg(const StringType& flag,
					const StringType& name, 
					const StringType& desc, 
					CmdLineInterfaceType& parser,
					int init,
					Visitor* v,
	                const AllocatorType& alloc )
: SwitchArg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, false, v, alloc),
_value( init ),
_default( init )
{ 
	parser.add( this );
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline bool MultiSwitchArg<T_Char, T_CharTraits, T_Alloc>::processArg(int *i, StringVectorType& args)
{
	if ( _ignoreable && Arg<T_Char, T_CharTraits, T_Alloc>::ignoreRest() )
		return false;

	if ( argMatches( args[*i] ))
	{
		// so the isSet() method will work
		_alreadySet = true;

		// Matched argument: increment value.
		++_value;

		_checkWithVisitor();

		return true;
	}
	else if ( combinedSwitchesMatch( args[*i] ) )
	{
		// so the isSet() method will work
		_alreadySet = true;

		// Matched argument: increment value.
		++_value;

		// Check for more in argument and increment value.
		while ( combinedSwitchesMatch( args[*i] ) ) 
			++_value;

		_checkWithVisitor();

		return false;
	}
	else
		return false;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto
MultiSwitchArg<T_Char, T_CharTraits, T_Alloc>::shortID(const StringType& val) const -> StringType
{
	return Arg<T_Char, T_CharTraits, T_Alloc>::shortID(val) + " ...";
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto
MultiSwitchArg<T_Char, T_CharTraits, T_Alloc>::longID(const StringType& val) const -> StringType
{
	return Arg<T_Char, T_CharTraits, T_Alloc>::longID(val) + "  (accepted multiple times)";
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void
MultiSwitchArg<T_Char, T_CharTraits, T_Alloc>::reset()
{
	MultiSwitchArg::_value = MultiSwitchArg::_default;
}

//////////////////////////////////////////////////////////////////////
//END MultiSwitchArg.cpp
//////////////////////////////////////////////////////////////////////

} //namespace TCLAP

#endif
