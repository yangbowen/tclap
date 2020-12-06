// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-


/****************************************************************************** 
 * 
 *  file:  XorHandler.h
 * 
 *  Copyright (c) 2003, Michael E. Smoot .
 *  Copyright (c) 2004, Michael E. Smoot, Daniel Aarno.
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

#ifndef TCLAP_XORHANDLER_H
#define TCLAP_XORHANDLER_H

#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include <tclap/Arg.h>

namespace TCLAP {
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class CmdLineInterface;
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class XorHandler;

/**
 * This class handles lists of Arg's that are to be XOR'd on the command
 * line.  This is used by CmdLine and you shouldn't ever use it.
 */
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class XorHandler
{
	public:
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using AllocatorType = T_Alloc;
		using StringType = std::basic_string<T_Char, T_CharTraits, T_Alloc>;
		using StringVectorType = std::vector<StringType, typename std::allocator_traits<AllocatorType>::template rebind_alloc<StringType>>;
		using OstreamType = std::basic_ostream<T_Char, T_CharTraits>;
		using ArgType = Arg<T_Char, T_CharTraits, T_Alloc>;
		using ArgListType = std::list<ArgType*, typename std::allocator_traits<AllocatorType>::template rebind_alloc<ArgType*>>;
		using ArgVectorType = std::vector<ArgType*, typename std::allocator_traits<AllocatorType>::template rebind_alloc<ArgType*>>;
		using ArgVectorVectorType = std::vector<ArgVectorType, typename std::allocator_traits<AllocatorType>::template rebind_alloc<ArgVectorType>>;
		using ArgListIteratorType = typename ArgListType::const_iterator;
		using ArgVectorIteratorType = typename ArgVectorType::const_iterator;
		using CmdLineInterfaceType = CmdLineInterface<T_Char, T_CharTraits, T_Alloc>;
		using XorHandlerType = XorHandler<T_Char, T_CharTraits, T_Alloc>;

	protected:

		/**
		 * The list of of lists of Arg's to be or'd together.
		 */
		ArgVectorVectorType _orList;

	public:

		/**
		 * Constructor.  Does nothing.
		 */
		XorHandler( ) : _orList(ArgVectorVectorType()) {}

		/**
		 * Add a list of Arg*'s that will be xor'd together.
		 * \param ors - list of Arg* that will be xor'd.
		 */
		void add( const ArgVectorType& ors );

		/**
		 * Checks whether the specified Arg is in one of the xor lists and
		 * if it does match one, returns the size of the xor list that the
		 * Arg matched.  If the Arg matches, then it also sets the rest of
		 * the Arg's in the list. You shouldn't use this.  
		 * \param a - The Arg to be checked.
		 */
		int check( const ArgType* a );

		/**
		 * Returns the XOR specific short usage.
		 */
		StringType shortUsage();

		/**
		 * Prints the XOR specific long usage.
		 * \param os - Stream to print to.
		 */
		void printLongUsage(OstreamType& os);

		/**
		 * Simply checks whether the Arg is contained in one of the arg
		 * lists.
		 * \param a - The Arg to be checked.
		 */
		bool contains( const ArgType* a );

		const ArgVectorVectorType& getXorList() const;

};


//////////////////////////////////////////////////////////////////////
//BEGIN XOR.cpp
//////////////////////////////////////////////////////////////////////
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void XorHandler<T_Char, T_CharTraits, T_Alloc>::add( const ArgVectorType& ors )
{
	_orList.push_back( ors );
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline int XorHandler<T_Char, T_CharTraits, T_Alloc>::check( const ArgType* a )
{
	// iterate over each XOR list
	for ( int i = 0; static_cast<unsigned int>(i) < _orList.size(); i++ )
	{
		// if the XOR list contains the arg..
		ArgVectorIteratorType ait = std::find( _orList[i].begin(), 
		                                   _orList[i].end(), a );
		if ( ait != _orList[i].end() )
		{
			// first check to see if a mutually exclusive switch
			// has not already been set
			for ( ArgVectorIteratorType it = _orList[i].begin(); 
				  it != _orList[i].end(); 
				  it++ )
				if ( a != (*it) && (*it)->isSet() )
					throw(CmdLineParseException(
					      "Mutually exclusive argument already set!",
					      (*it)->toString()));

			// go through and set each arg that is not a
			for ( ArgVectorIteratorType it = _orList[i].begin(); 
				  it != _orList[i].end(); 
				  it++ )
				if ( a != (*it) )
					(*it)->xorSet();

			// return the number of required args that have now been set
			if ( (*ait)->allowMore() )
				return 0;
			else
				return static_cast<int>(_orList[i].size());
		}
	}

	if ( a->isRequired() )
		return 1;
	else
		return 0;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline bool XorHandler<T_Char, T_CharTraits, T_Alloc>::contains( const ArgType* a )
{
	for ( int i = 0; static_cast<unsigned int>(i) < _orList.size(); i++ )
		for ( ArgVectorIteratorType it = _orList[i].begin(); 
			  it != _orList[i].end(); 
			  it++ )	
			if ( a == (*it) )
				return true;

	return false;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline auto XorHandler<T_Char, T_CharTraits, T_Alloc>::getXorList() const -> const ArgVectorVectorType&
{
	return _orList;
}



//////////////////////////////////////////////////////////////////////
//END XOR.cpp
//////////////////////////////////////////////////////////////////////

} //namespace TCLAP

#endif 
