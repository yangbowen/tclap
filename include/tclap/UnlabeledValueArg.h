// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-


/******************************************************************************
 *
 *  file:  UnlabeledValueArg.h
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


#ifndef TCLAP_UNLABELED_VALUE_ARGUMENT_H
#define TCLAP_UNLABELED_VALUE_ARGUMENT_H

#include <string>
#include <vector>

#include <tclap/StringConvert.h>
#include <tclap/ValueArg.h>
#include <tclap/OptionalUnlabeledTracker.h>


namespace TCLAP {

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	class UnlabeledValueArg;

	/**
	 * The basic unlabeled argument that parses a value.
	 * This is a template class, which means the type T defines the type
	 * that a given object will attempt to parse when an UnlabeledValueArg
	 * is reached in the list of args that the CmdLine iterates over.
	 */
	template<class T, typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class UnlabeledValueArg : public ValueArg<T, T_Char, T_CharTraits, T_Alloc> {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::StringConvertType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::StringViewType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::StringType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::StringVectorType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgListType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgVectorType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgListIteratorType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgVectorIteratorType;
		using typename Arg<T_Char, T_CharTraits, T_Alloc>::CmdLineInterfaceType;
		using typename ValueArg<T, T_Char, T_CharTraits, T_Alloc>::ConstraintType;
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
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::processArg;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::getValue;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::operator const T&;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::shortID;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::longID;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::reset;

		/**
		 * UnlabeledValueArg constructor.
		 * \param name - A one word name for the argument.  Note that this is used for
		 * identification, not as a long flag.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param value - The default value assigned to this argument if it
		 * is not present on the command line.
		 * \param typeDesc - A short, human readable description of the
		 * type that this object expects.  This is used in the generation
		 * of the USAGE statement.  The goal is to be helpful to the end user
		 * of the program.
		 * \param ignoreable - Allows you to specify that this argument can be
		 * ignored if the '--' flag is set.  This defaults to false (cannot
		 * be ignored) and should  generally stay that way unless you have
		 * some special need for certain arguments to be ignored.
		 * \param v - Optional Visitor.  You should leave this blank unless
		 * you have a very good reason.
		 */
		UnlabeledValueArg(const StringType& name,
			const StringType& desc,
			bool req,
			T value,
			const StringType& typeDesc,
			bool ignoreable = false,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());

		/**
		 * UnlabeledValueArg constructor.
		 * \param name - A one word name for the argument.  Note that this is used for
		 * identification, not as a long flag.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param value - The default value assigned to this argument if it
		 * is not present on the command line.
		 * \param typeDesc - A short, human readable description of the
		 * type that this object expects.  This is used in the generation
		 * of the USAGE statement.  The goal is to be helpful to the end user
		 * of the program.
		 * \param parser - A CmdLine parser object to add this Arg to
		 * \param ignoreable - Allows you to specify that this argument can be
		 * ignored if the '--' flag is set.  This defaults to false (cannot
		 * be ignored) and should  generally stay that way unless you have
		 * some special need for certain arguments to be ignored.
		 * \param v - Optional Visitor.  You should leave this blank unless
		 * you have a very good reason.
		 */
		UnlabeledValueArg(const StringType& name,
			const StringType& desc,
			bool req,
			T value,
			const StringType& typeDesc,
			CmdLineInterfaceType& parser,
			bool ignoreable = false,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());

		/**
		 * UnlabeledValueArg constructor.
		 * \param name - A one word name for the argument.  Note that this is used for
		 * identification, not as a long flag.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param value - The default value assigned to this argument if it
		 * is not present on the command line.
		 * \param constraint - A pointer to a Constraint object used
		 * to constrain this Arg.
		 * \param ignoreable - Allows you to specify that this argument can be
		 * ignored if the '--' flag is set.  This defaults to false (cannot
		 * be ignored) and should  generally stay that way unless you have
		 * some special need for certain arguments to be ignored.
		 * \param v - Optional Visitor.  You should leave this blank unless
		 * you have a very good reason.
		 */
		UnlabeledValueArg(const StringType& name,
			const StringType& desc,
			bool req,
			T value,
			ConstraintType* constraint,
			bool ignoreable = false,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());


		/**
		 * UnlabeledValueArg constructor.
		 * \param name - A one word name for the argument.  Note that this is used for
		 * identification, not as a long flag.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param value - The default value assigned to this argument if it
		 * is not present on the command line.
		 * \param constraint - A pointer to a Constraint object used
		 * to constrain this Arg.
		 * \param parser - A CmdLine parser object to add this Arg to
		 * \param ignoreable - Allows you to specify that this argument can be
		 * ignored if the '--' flag is set.  This defaults to false (cannot
		 * be ignored) and should  generally stay that way unless you have
		 * some special need for certain arguments to be ignored.
		 * \param v - Optional Visitor.  You should leave this blank unless
		 * you have a very good reason.
		 */
		UnlabeledValueArg(const StringType& name,
			const StringType& desc,
			bool req,
			T value,
			ConstraintType* constraint,
			CmdLineInterfaceType& parser,
			bool ignoreable = false,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());

		/**
		 * Handles the processing of the argument.
		 * This re-implements the Arg version of this method to set the
		 * _value of the argument appropriately.  Handling specific to
		 * unlabeled arguments.
		 * \param idx_arg - Pointer the the current argument in the list.
		 * \param args - Mutable list of strings.
		 */
		virtual bool processArg(std::size_t& idx_arg, StringVectorType& args) override;

		/**
		 * Overrides shortID for specific behavior.
		 */
		virtual StringType shortID(const StringType& val = StringConvertType::fromConstBasicCharString("val")) const override;

		/**
		 * Overrides longID for specific behavior.
		 */
		virtual StringType longID(const StringType& val = StringConvertType::fromConstBasicCharString("val")) const override;

		/**
		 * Overrides operator== for specific behavior.
		 */
		virtual bool operator==(const ArgType& a) const override;

		/**
		 * Instead of pushing to the front of list, push to the back.
		 * \param argList - The list to add this to.
		 */
		virtual void addToList(ArgListType& argList) const override;

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
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::_value;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::_default;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::_typeDesc;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::_constraint;
		using ValueArg<T, T_Char, T_CharTraits, T_Alloc>::_extractValue;
	};

	/**
	 * Constructor implementation.
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::UnlabeledValueArg(const StringType& name,
		const StringType& desc,
		bool req,
		T val,
		const StringType& typeDesc,
		bool ignoreable,
		Visitor* v,
		const AllocatorType& alloc)
		: ValueArg<T, T_Char, T_CharTraits, T_Alloc>(StringType(), name, desc, req, val, typeDesc, v, alloc) {
		_ignoreable = ignoreable;

		OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check(req, toString());

	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::UnlabeledValueArg(const StringType& name,
		const StringType& desc,
		bool req,
		T val,
		const StringType& typeDesc,
		CmdLineInterfaceType& parser,
		bool ignoreable,
		Visitor* v,
		const AllocatorType& alloc)
		: ValueArg<T, T_Char, T_CharTraits, T_Alloc>(StringType(), name, desc, req, val, typeDesc, v, alloc) {
		_ignoreable = ignoreable;
		OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check(req, toString());
		parser.add(this);
	}

	/**
	 * Constructor implementation.
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::UnlabeledValueArg(const StringType& name,
		const StringType& desc,
		bool req,
		T val,
		ConstraintType* constraint,
		bool ignoreable,
		Visitor* v,
		const AllocatorType& alloc)
		: ValueArg<T, T_Char, T_CharTraits, T_Alloc>(StringType(), name, desc, req, val, constraint, v, alloc) {
		_ignoreable = ignoreable;
		OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check(req, toString());
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::UnlabeledValueArg(const StringType& name,
		const StringType& desc,
		bool req,
		T val,
		ConstraintType* constraint,
		CmdLineInterfaceType& parser,
		bool ignoreable,
		Visitor* v,
		const AllocatorType& alloc)
		: ValueArg<T, T_Char, T_CharTraits, T_Alloc>(StringType(), name, desc, req, val, constraint, v, alloc) {
		_ignoreable = ignoreable;
		OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check(req, toString());
		parser.add(this);
	}

	/**
	 * Implementation of processArg().
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	bool UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::processArg(std::size_t& idx_arg, StringVectorType& args) {

		if (_alreadySet)
			return false;

		if (_hasBlanks(args[idx_arg]))
			return false;

		// never ignore an unlabeled arg

		_extractValue(args[idx_arg]);
		_alreadySet = true;
		return true;
	}

	/**
	 * Overriding shortID for specific output.
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	auto UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::shortID(const StringType& val) const -> StringType {
		static_cast<void>(val); // Ignore input, don't warn
		return StringConvertType::fromConstBasicCharString("<") + _typeDesc + StringConvertType::fromConstBasicCharString(">");
	}

	/**
	 * Overriding longID for specific output.
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	auto UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::longID(const StringType& val) const -> StringType {
		static_cast<void>(val); // Ignore input, don't warn

		// Ideally we would like to be able to use RTTI to return the name
		// of the type required for this argument.  However, g++ at least, 
		// doesn't appear to return terribly useful "names" of the types.  
		return StringConvertType::fromConstBasicCharString("<") + _typeDesc + StringConvertType::fromConstBasicCharString(">");
	}

	/**
	 * Overriding operator== for specific behavior.
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	bool UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::operator==(const ArgType& a) const {
		if (_name == a.getName() || _description == a.getDescription())
			return true;
		else
			return false;
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	void UnlabeledValueArg<T, T_Char, T_CharTraits, T_Alloc>::addToList(ArgListType& argList) const {
		argList.push_back(const_cast<ArgType*>(static_cast<const ArgType* const>(this)));
	}

}
#endif
