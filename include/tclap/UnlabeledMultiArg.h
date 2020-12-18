// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-


/****************************************************************************** 
 * 
 *  file:  UnlabeledMultiArg.h
 * 
 *  Copyright (c) 2003, Michael E. Smoot.
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


#ifndef TCLAP_MULTIPLE_UNLABELED_ARGUMENT_H
#define TCLAP_MULTIPLE_UNLABELED_ARGUMENT_H

#include <string>
#include <vector>

#include <tclap/MultiArg.h>
#include <tclap/OptionalUnlabeledTracker.h>

namespace TCLAP {

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
class UnlabeledMultiArg;

/**
 * Just like a MultiArg, except that the arguments are unlabeled.  Basically,
 * this Arg will slurp up everything that hasn't been matched to another 
 * Arg.
 */
template<class T, typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class UnlabeledMultiArg : public MultiArg<T, T_Char, T_CharTraits, T_Alloc>
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
		using typename MultiArg<T, T_Char, T_CharTraits, T_Alloc>::ConstraintType;
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
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::processArg;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::getValue;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::begin;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::end;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::shortID;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::longID;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::isRequired;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::allowMore;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::reset;

		/**
		 * Constructor.  
		 * \param name - The name of the Arg. Note that this is used for
		 * identification, not as a long flag.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 *  line.
		 * \param typeDesc - A short, human readable description of the
		 * type that this object expects.  This is used in the generation
		 * of the USAGE statement.  The goal is to be helpful to the end user
		 * of the program.
		 * \param ignoreable - Whether or not this argument can be ignored
		 * using the "--" flag.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		UnlabeledMultiArg( const StringType& name,
				           const StringType& desc,
						   bool req,
				           const StringType& typeDesc,
						   bool ignoreable = false,
				           Visitor* v = nullptr,
						   const AllocatorType& alloc = AllocatorType() );
		/**
		 * Constructor.  
		 * \param name - The name of the Arg. Note that this is used for
		 * identification, not as a long flag.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 *  line.
		 * \param typeDesc - A short, human readable description of the
		 * type that this object expects.  This is used in the generation
		 * of the USAGE statement.  The goal is to be helpful to the end user
		 * of the program.
		 * \param parser - A CmdLine parser object to add this Arg to
		 * \param ignoreable - Whether or not this argument can be ignored
		 * using the "--" flag.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		UnlabeledMultiArg( const StringType& name,
				           const StringType& desc,
						   bool req,
				           const StringType& typeDesc,
						   CmdLineInterfaceType& parser,
						   bool ignoreable = false,
				           Visitor* v = nullptr,
						   const AllocatorType& alloc = AllocatorType() );
						 
		/**
		 * Constructor.  
		 * \param name - The name of the Arg. Note that this is used for
		 * identification, not as a long flag.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 *  line.
		 * \param constraint - A pointer to a Constraint object used
		 * to constrain this Arg.
		 * \param ignoreable - Whether or not this argument can be ignored
		 * using the "--" flag.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		UnlabeledMultiArg( const StringType& name,
						   const StringType& desc,
						   bool req,
						   ConstraintType* constraint,
						   bool ignoreable = false,
						   Visitor* v = nullptr,
						   const AllocatorType& alloc = AllocatorType() );

		/**
		 * Constructor.  
		 * \param name - The name of the Arg. Note that this is used for
		 * identification, not as a long flag.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 *  line.
		 * \param constraint - A pointer to a Constraint object used
		 * to constrain this Arg.
		 * \param parser - A CmdLine parser object to add this Arg to
		 * \param ignoreable - Whether or not this argument can be ignored
		 * using the "--" flag.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		UnlabeledMultiArg( const StringType& name, 
						   const StringType& desc, 
						   bool req,
						   ConstraintType* constraint,
						   CmdLineInterfaceType& parser,
						   bool ignoreable = false,
						   Visitor* v = nullptr,
						   const AllocatorType& alloc = AllocatorType() );
						 
		/**
		 * Handles the processing of the argument.
		 * This re-implements the Arg version of this method to set the
		 * _value of the argument appropriately.  It knows the difference
		 * between labeled and unlabeled.
		 * \param i - Pointer the the current argument in the list.
		 * \param args - Mutable list of strings. Passed from main().
		 */
		virtual bool processArg(int* i, StringVectorType& args); 

		/**
		 * Returns the a short id string.  Used in the usage.
		 * \param val - value to be used.
		 */
		virtual StringType shortID(const StringType& val="val") const;

		/**
		 * Returns the a long id string.  Used in the usage.
		 * \param val - value to be used.
		 */
		virtual StringType longID(const StringType& val="val") const;

		/**
		 * Operator ==.
		 * \param a - The Arg to be compared to this.
		 */
		virtual bool operator==(const ArgType& a) const;

		/**
		 * Pushes this to back of list rather than front.
		 * \param argList - The list this should be added to.
		 */
		virtual void addToList( ArgListType& argList ) const;

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
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::_values;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::_typeDesc;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::_constraint;
		using MultiArg<T, T_Char, T_CharTraits, T_Alloc>::_extractValue;
};

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::UnlabeledMultiArg(const StringType& name,
				                        const StringType& desc, 
										bool req,
					                    const StringType& typeDesc,
										bool ignoreable,
					                    Visitor* v,
										const AllocatorType& alloc)
: MultiArg<T, T_Char, T_CharTraits, T_Alloc>(StringType(), name, desc,  req, typeDesc, v, alloc)
{ 
	_ignoreable = ignoreable;
	OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check(true, toString());
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::UnlabeledMultiArg(const StringType& name,
				                        const StringType& desc, 
										bool req,
					                    const StringType& typeDesc,
										CmdLineInterfaceType& parser,
										bool ignoreable,
					                    Visitor* v,
										const AllocatorType& alloc)
: MultiArg<T, T_Char, T_CharTraits, T_Alloc>(StringType(), name, desc,  req, typeDesc, v, alloc)
{ 
	_ignoreable = ignoreable;
	OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check(true, toString());
	parser.add( this );
}


template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::UnlabeledMultiArg(const StringType& name,
				                        const StringType& desc, 
										bool req,
					                    ConstraintType* constraint,
										bool ignoreable,
					                    Visitor* v,
										const AllocatorType& alloc)
: MultiArg<T, T_Char, T_CharTraits, T_Alloc>(StringType(), name, desc,  req, constraint, v, alloc)
{ 
	_ignoreable = ignoreable;
	OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check(true, toString());
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::UnlabeledMultiArg(const StringType& name,
				                        const StringType& desc, 
										bool req,
					                    ConstraintType* constraint,
										CmdLineInterfaceType& parser,
										bool ignoreable,
					                    Visitor* v,
										const AllocatorType& alloc)
: MultiArg<T, T_Char, T_CharTraits, T_Alloc>(StringType(), name, desc,  req, constraint, v, alloc)
{ 
	_ignoreable = ignoreable;
	OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check(true, toString());
	parser.add( this );
}


template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
bool UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::processArg(int *i, StringVectorType& args)
{

	if ( _hasBlanks( args[*i] ) )
		return false;

	// never ignore an unlabeled multi arg


	// always take the first value, regardless of the start string 
	_extractValue( args[(*i)] );

	/*
	// continue taking args until we hit the end or a start string 
	while ( (unsigned int)(*i)+1 < args.size() &&
			args[(*i)+1].find_first_of( Arg::flagStartString() ) != 0 &&
            args[(*i)+1].find_first_of( Arg::nameStartString() ) != 0 ) 
		_extractValue( args[++(*i)] );
	*/

	_alreadySet = true;

	return true;
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
auto UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::shortID(const StringType& val) const -> StringType
{
	static_cast<void>(val); // Ignore input, don't warn
	return StringType("<") + _typeDesc + "> ...";
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
auto UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::longID(const StringType& val) const -> StringType
{
	static_cast<void>(val); // Ignore input, don't warn
	return StringType("<") + _typeDesc + ">  (accepted multiple times)";
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
bool UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::operator==(const ArgType& a) const
{
	if ( _name == a.getName() || _description == a.getDescription() )
		return true;
	else
		return false;
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
void UnlabeledMultiArg<T, T_Char, T_CharTraits, T_Alloc>::addToList( ArgListType& argList ) const
{
	argList.push_back( const_cast<ArgType*>(static_cast<const ArgType* const>(this)) );
}

}

#endif
