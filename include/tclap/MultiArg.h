// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  MultiArg.h
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


#ifndef TCLAP_MULTIPLE_ARGUMENT_H
#define TCLAP_MULTIPLE_ARGUMENT_H

#include <string>
#include <vector>

#include <tclap/StringConvert.h>
#include <tclap/Arg.h>
#include <tclap/Constraint.h>

namespace TCLAP {

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	class Constraint;
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	class MultiArg;

	/**
	 * An argument that allows multiple values of type T to be specified.  Very
	 * similar to a ValueArg, except a vector of values will be returned
	 * instead of just one.
	 */
	template<class T, typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class MultiArg : public Arg<T_Char, T_CharTraits, T_Alloc> {
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
		using ConstraintType = Constraint<T, T_Char, T_CharTraits, T_Alloc>;
		using container_type = std::vector<T, typename std::allocator_traits<AllocatorType>::template rebind_alloc<T>>;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
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

		MultiArg(const MultiArg& rhs) = delete;
		MultiArg& operator=(const MultiArg& rhs) = delete;

		/**
		 * Constructor.
		 * \param flag - The one character flag that identifies this
		 * argument on the command line.
		 * \param name - A one word name for the argument.  Can be
		 * used as a long flag on the command line.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param typeDesc - A short, human readable description of the
		 * type that this object expects.  This is used in the generation
		 * of the USAGE statement.  The goal is to be helpful to the end user
		 * of the program.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		MultiArg(const StringType& flag,
			const StringType& name,
			const StringType& desc,
			bool req,
			const StringType& typeDesc,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());

		/**
		 * Constructor.
		 * \param flag - The one character flag that identifies this
		 * argument on the command line.
		 * \param name - A one word name for the argument.  Can be
		 * used as a long flag on the command line.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param typeDesc - A short, human readable description of the
		 * type that this object expects.  This is used in the generation
		 * of the USAGE statement.  The goal is to be helpful to the end user
		 * of the program.
		 * \param parser - A CmdLine parser object to add this Arg to
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		MultiArg(const StringType& flag,
			const StringType& name,
			const StringType& desc,
			bool req,
			const StringType& typeDesc,
			CmdLineInterfaceType& parser,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());

		/**
		 * Constructor.
		 * \param flag - The one character flag that identifies this
		 * argument on the command line.
		 * \param name - A one word name for the argument.  Can be
		 * used as a long flag on the command line.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param constraint - A pointer to a Constraint object used
		 * to constrain this Arg.
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		MultiArg(const StringType& flag,
			const StringType& name,
			const StringType& desc,
			bool req,
			ConstraintType* constraint,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());

		/**
		 * Constructor.
		 * \param flag - The one character flag that identifies this
		 * argument on the command line.
		 * \param name - A one word name for the argument.  Can be
		 * used as a long flag on the command line.
		 * \param desc - A description of what the argument is for or
		 * does.
		 * \param req - Whether the argument is required on the command
		 * line.
		 * \param constraint - A pointer to a Constraint object used
		 * to constrain this Arg.
		 * \param parser - A CmdLine parser object to add this Arg to
		 * \param v - An optional visitor.  You probably should not
		 * use this unless you have a very good reason.
		 */
		MultiArg(const StringType& flag,
			const StringType& name,
			const StringType& desc,
			bool req,
			ConstraintType* constraint,
			CmdLineInterfaceType& parser,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());

		/**
		 * Handles the processing of the argument.
		 * This re-implements the Arg version of this method to set the
		 * _value of the argument appropriately.  It knows the difference
		 * between labeled and unlabeled.
		 * \param idx_arg - Pointer the the current argument in the list.
		 * \param args - Mutable list of strings. Passed from main().
		 */
		virtual bool processArg(std::size_t& idx_arg, StringVectorType& args) override;

		/**
		 * Returns a vector of type T containing the values parsed from
		 * the command line.
		 */
		const container_type& getValue() const { return _values; }

		/**
		 * Returns an iterator over the values parsed from the command
		 * line.
		 */
		const_iterator begin() const { return _values.begin(); }

		/**
		 * Returns the end of the values parsed from the command
		 * line.
		 */
		const_iterator end() const { return _values.end(); }

		/**
		 * Returns the a short id string.  Used in the usage.
		 * \param val - value to be used.
		 */
		virtual StringType shortID(const StringType& val = StringConvertType::fromConstBasicCharString("val")) const override;

		/**
		 * Returns the a long id string.  Used in the usage.
		 * \param val - value to be used.
		 */
		virtual StringType longID(const StringType& val = StringConvertType::fromConstBasicCharString("val")) const override;

		/**
		 * Once we've matched the first value, then the arg is no longer
		 * required.
		 */
		virtual bool isRequired() const override;

		virtual bool allowMore() override;

		virtual void reset() override;

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
		 * The list of values parsed from the CmdLine.
		 */
		container_type _values;

		/**
		 * The description of type T to be used in the usage.
		 */
		StringType _typeDesc;

		/**
		 * A list of constraint on this Arg.
		 */
		ConstraintType* _constraint;

		/**
		 * Extracts the value from the string.
		 * Attempts to parse string as type T, if this fails an exception
		 * is thrown.
		 * \param val - The string to be read.
		 */
		void _extractValue(const StringType& val);

		/**
		 * Used by XorHandler to decide whether to keep parsing for this arg.
		 */
		bool _allowMore;
	};

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	MultiArg<T, T_Char, T_CharTraits, T_Alloc>::MultiArg(const StringType& flag,
		const StringType& name,
		const StringType& desc,
		bool req,
		const StringType& typeDesc,
		Visitor* v,
		const AllocatorType& alloc)
		: Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, req, true, v, alloc),
		_values(container_type()),
		_typeDesc(typeDesc),
		_constraint(nullptr),
		_allowMore(false) {
		_acceptsMultipleValues = true;
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	MultiArg<T, T_Char, T_CharTraits, T_Alloc>::MultiArg(const StringType& flag,
		const StringType& name,
		const StringType& desc,
		bool req,
		const StringType& typeDesc,
		CmdLineInterfaceType& parser,
		Visitor* v,
		const AllocatorType& alloc)
		: Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, req, true, v, alloc),
		_values(container_type()),
		_typeDesc(typeDesc),
		_constraint(nullptr),
		_allowMore(false) {
		parser.add(this);
		_acceptsMultipleValues = true;
	}

	/**
	 *
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	MultiArg<T, T_Char, T_CharTraits, T_Alloc>::MultiArg(const StringType& flag,
		const StringType& name,
		const StringType& desc,
		bool req,
		ConstraintType* constraint,
		Visitor* v,
		const AllocatorType& alloc)
		: Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, req, true, v, alloc),
		_values(container_type()),
		_typeDesc(ConstraintType::shortID(constraint)),
		_constraint(constraint),
		_allowMore(false) {
		_acceptsMultipleValues = true;
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	MultiArg<T, T_Char, T_CharTraits, T_Alloc>::MultiArg(const StringType& flag,
		const StringType& name,
		const StringType& desc,
		bool req,
		ConstraintType* constraint,
		CmdLineInterfaceType& parser,
		Visitor* v,
		const AllocatorType& alloc)
		: Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, req, true, v, alloc),
		_values(container_type()),
		_typeDesc(ConstraintType::shortID(constraint)),
		_constraint(constraint),
		_allowMore(false) {
		parser.add(this);
		_acceptsMultipleValues = true;
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	bool MultiArg<T, T_Char, T_CharTraits, T_Alloc>::processArg(std::size_t& idx_arg, StringVectorType& args) {
		if (_ignoreable && Arg<T_Char, T_CharTraits, T_Alloc>::ignoreRest())
			return false;

		if (_hasBlanks(args[idx_arg]))
			return false;

		StringType flag = args[idx_arg];
		StringType value;

		trimFlag(flag, value);

		if (argMatches(flag))    	{
			if (Arg<T_Char, T_CharTraits, T_Alloc>::delimiter() != StringConvertType::fromConstBasicChar(' ') && value.empty())
				throw(ArgParseException<T_Char, T_CharTraits, T_Alloc>(StringConvertType::fromConstBasicCharString("Couldn't find delimiter for this argument!"), toString()));

			// always take the first one, regardless of start string
			if (value.empty()) 		{
				(idx_arg)++;
				if (idx_arg < args.size())
					_extractValue(args[idx_arg]);
				else
					throw(ArgParseException<T_Char, T_CharTraits, T_Alloc>(StringConvertType::fromConstBasicCharString("Missing a value for this argument!"), toString()));
			} 		else
				_extractValue(value);

			/*
			// continuing taking the args until we hit one with a start string
			while ( idx_arg+1 < args.size() &&
					args[idx_arg+1].find_first_of( Arg::flagStartString() ) != 0 &&
					args[idx_arg+1].find_first_of( Arg::nameStartString() ) != 0 )
					_extractValue( args[++idx_arg] );
			*/

			_alreadySet = true;
			_checkWithVisitor();

			return true;
		} 	else
			return false;
	}

	/**
	 *
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	auto MultiArg<T, T_Char, T_CharTraits, T_Alloc>::shortID(const StringType& val) const -> StringType {
		static_cast<void>(val); // Ignore input, don't warn
		return Arg<T_Char, T_CharTraits, T_Alloc>::shortID(_typeDesc) + StringConvertType::fromConstBasicCharString(" ...");
	}

	/**
	 *
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	auto MultiArg<T, T_Char, T_CharTraits, T_Alloc>::longID(const StringType& val) const -> StringType {
		static_cast<void>(val); // Ignore input, don't warn
		return Arg<T_Char, T_CharTraits, T_Alloc>::longID(_typeDesc) + StringConvertType::fromConstBasicCharString("  (accepted multiple times)");
	}

	/**
	 * Once we've matched the first value, then the arg is no longer
	 * required.
	 */
	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	bool MultiArg<T, T_Char, T_CharTraits, T_Alloc>::isRequired() const {
		if (_required) 	{
			if (_values.size() > 1)
				return false;
			else
				return true;
		}    	else
			return false;

	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	void MultiArg<T, T_Char, T_CharTraits, T_Alloc>::_extractValue(const StringType& val) {
		try {
			T tmp;
			ExtractValue(tmp, val, typename ArgTraits<T>::ValueCategory());
			_values.push_back(tmp);
		} catch (ArgParseException<T_Char, T_CharTraits, T_Alloc>& e) {
			throw ArgParseException<T_Char, T_CharTraits, T_Alloc>(e.error(), toString());
		}

		if (_constraint)
			if (!_constraint->check(_values.back()))
				throw(CmdLineParseException<T_Char, T_CharTraits, T_Alloc>(
					StringConvertType::fromConstBasicCharString("Value \'") + val + StringConvertType::fromConstBasicCharString("\' does not meet constraint: ") + _constraint->description(),
					toString()
					));
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	bool MultiArg<T, T_Char, T_CharTraits, T_Alloc>::allowMore() {
		bool am = _allowMore;
		_allowMore = true;
		return am;
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	void MultiArg<T, T_Char, T_CharTraits, T_Alloc>::reset() {
		Arg<T_Char, T_CharTraits, T_Alloc>::reset();
		_values.clear();
	}

} // namespace TCLAP

#endif
