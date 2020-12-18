// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  Arg.h
 *
 *  Copyright (c) 2003, Michael E. Smoot .
 *  Copyright (c) 2004, Michael E. Smoot, Daniel Aarno .
 *  Copyright (c) 2017 Google Inc.
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


#ifndef TCLAP_ARGUMENT_H
#define TCLAP_ARGUMENT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>

#include <tclap/UseAllocatorBase.h>
#include <tclap/StringConvert.h>
#include <tclap/ArgException.h>
#include <tclap/Visitor.h>
#include <tclap/CmdLineInterface.h>
#include <tclap/ArgTraits.h>
#include <tclap/StandardTraits.h>

namespace TCLAP {

	class Visitor;
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class Arg;
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class CmdLineInterface;

	/**
	 * A virtual base class that defines the essential data for all arguments.
	 * This class, or one of its existing children, must be subclassed to do
	 * anything.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class Arg : public UseAllocatorBase<T_Alloc> {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using StringConvertType = StringConvert<T_Char, T_CharTraits>;
		using StringType = std::basic_string<T_Char, T_CharTraits, T_Alloc>;
		using StringVectorType = std::vector<StringType, typename std::allocator_traits<AllocatorType>::template rebind_alloc<StringType>>;
		using ArgType = Arg<T_Char, T_CharTraits, T_Alloc>;
		using ArgListType = std::list<ArgType*, typename std::allocator_traits<AllocatorType>::template rebind_alloc<ArgType*>>;
		using ArgVectorType = std::vector<ArgType*, typename std::allocator_traits<AllocatorType>::template rebind_alloc<ArgType*>>;
		using ArgListIteratorType = typename ArgListType::const_iterator;
		using ArgVectorIteratorType = typename ArgVectorType::const_iterator;
		using CmdLineInterfaceType = CmdLineInterface<T_Char, T_CharTraits, T_Alloc>;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		Arg(const Arg& rhs) = delete;
		Arg& operator=(const Arg& rhs) = delete;

		/**
		 * Destructor.
		 */
		virtual ~Arg();

		/**
		 * Adds this to the specified list of Args.
		 * \param argList - The list to add this to.
		 */
		virtual void addToList(ArgListType& argList) const;

		/**
		 * Begin ignoring arguments since the "--" argument was specified.
		 */
		static void beginIgnoring() { ignoreRestRef() = true; }

		/**
		 * Whether to ignore the rest.
		 */
		static bool ignoreRest() { return ignoreRestRef(); }

		/**
		 * The delimiter that separates an argument flag/name from the
		 * value.
		 */
		static CharType delimiter() { return delimiterRef(); }

		/**
		 * The char used as a place holder when SwitchArgs are combined.
		 * Currently set to the bell char (ASCII 7).
		 */
		// TODO: Make blankChar() charset-independent.
		static CharType blankChar() { return (CharType)7; }

		/**
		 * The char that indicates the beginning of a flag.  Defaults to '-', but
		 * clients can define TCLAP_FLAGSTARTCHAR to override.
		 */
#ifndef TCLAP_FLAGSTARTCHAR
#define TCLAP_FLAGSTARTCHAR '-'
#endif
		static CharType flagStartChar() { return StringConvertType::fromConstBasicChar(TCLAP_FLAGSTARTCHAR); }

		/**
		 * The sting that indicates the beginning of a flag.  Defaults to "-", but
		 * clients can define TCLAP_FLAGSTARTSTRING to override. Should be the same
		 * as TCLAP_FLAGSTARTCHAR.
		 */
#ifndef TCLAP_FLAGSTARTSTRING
#define TCLAP_FLAGSTARTSTRING "-"
#endif
		static const StringType flagStartString() { return StringConvertType::fromConstBasicCharString(TCLAP_FLAGSTARTSTRING); }

		/**
		 * The sting that indicates the beginning of a name.  Defaults to "--", but
		 *  clients can define TCLAP_NAMESTARTSTRING to override.
		 */
#ifndef TCLAP_NAMESTARTSTRING
#define TCLAP_NAMESTARTSTRING "--"
#endif
		static const StringType nameStartString() { return StringConvertType::fromConstBasicCharString(TCLAP_NAMESTARTSTRING); }

		/**
		 * The name used to identify the ignore rest argument.
		 */
		static const StringType ignoreNameString() { return StringConvertType::fromConstBasicCharString("ignore_rest"); }

		/**
		 * Sets the delimiter for all arguments.
		 * \param c - The character that delimits flags/names from values.
		 */
		static void setDelimiter(CharType c) { delimiterRef() = c; }

		/**
		 * Pure virtual method meant to handle the parsing and value assignment
		 * of the string on the command line.
		 * \param i - Pointer the the current argument in the list.
		 * \param args - Mutable list of strings. What is
		 * passed in from main.
		 */
		virtual bool processArg(int* i, StringVectorType& args) = 0;

		/**
		 * Operator ==.
		 * Equality operator. Must be virtual to handle unlabeled args.
		 * \param a - The Arg to be compared to this.
		 */
		virtual bool operator==(const Arg& a) const;

		/**
		 * Returns the argument flag.
		 */
		const StringType& getFlag() const;

		/**
		 * Returns the argument name.
		 */
		const StringType& getName() const;

		/**
		 * Returns the argument description.
		 */
		StringType getDescription() const;

		/**
		 * Indicates whether the argument is required.
		 */
		virtual bool isRequired() const;

		/**
		 * Sets _required to true. This is used by the XorHandler.
		 * You really have no reason to ever use it.
		 */
		void forceRequired();

		/**
		 * Sets the _alreadySet value to true.  This is used by the XorHandler.
		 * You really have no reason to ever use it.
		 */
		void xorSet();

		/**
		 * Indicates whether a value must be specified for argument.
		 */
		bool isValueRequired() const;

		/**
		 * Indicates whether the argument has already been set.  Only true
		 * if the arg has been matched on the command line.
		 */
		bool isSet() const;

		/**
		 * Indicates whether the argument can be ignored, if desired.
		 */
		bool isIgnoreable() const;

		/**
		 * A method that tests whether a string matches this argument.
		 * This is generally called by the processArg() method.  This
		 * method could be re-implemented by a child to change how
		 * arguments are specified on the command line.
		 * \param s - The string to be compared to the flag/name to determine
		 * whether the arg matches.
		 */
		virtual bool argMatches(const StringType& s) const;

		/**
		 * Returns a simple string representation of the argument.
		 * Primarily for debugging.
		 */
		virtual StringType toString() const;

		/**
		 * Returns a short ID for the usage.
		 * \param valueId - The value used in the id.
		 */
		virtual StringType shortID(const StringType& valueId = StringConvertType::fromConstBasicCharString("val")) const;

		/**
		 * Returns a long ID for the usage.
		 * \param valueId - The value used in the id.
		 */
		virtual StringType longID(const StringType& valueId = StringConvertType::fromConstBasicCharString("val")) const;

		/**
		 * Trims a value off of the flag.
		 * \param flag - The string from which the flag and value will be
		 * trimmed. Contains the flag once the value has been trimmed.
		 * \param value - Where the value trimmed from the string will
		 * be stored.
		 */
		virtual void trimFlag(StringType& flag, StringType& value) const;

		/**
		 * Checks whether a given string has blank chars, indicating that
		 * it is a combined SwitchArg.  If so, return true, otherwise return
		 * false.
		 * \param s - string to be checked.
		 */
		bool _hasBlanks(const StringType& s) const;

		/**
		 * Sets the requireLabel. Used by XorHandler.  You shouldn't ever
		 * use this.
		 * \param s - Set the requireLabel to this value.
		 */
		void setRequireLabel(const StringType& s);

		/**
		 * Used for MultiArgs and XorHandler to determine whether args
		 * can still be set.
		 */
		virtual bool allowMore();

		/**
		 * Use by output classes to determine whether an Arg accepts
		 * multiple values.
		 */
		virtual bool acceptsMultipleValues();

		/**
		 * Clears the Arg object and allows it to be reused by new
		 * command lines.
		 */
		virtual void reset();

	protected:
		/**
		 * The single char flag used to identify the argument.
		 * This value (preceded by a dash {-}), can be used to identify
		 * an argument on the command line.  The _flag can be blank,
		 * in fact this is how unlabeled args work.  Unlabeled args must
		 * override appropriate functions to get correct handling. Note
		 * that the _flag does NOT include the dash as part of the flag.
		 */
		StringType _flag;

		/**
		 * A single word namd identifying the argument.
		 * This value (preceded by two dashed {--}) can also be used
		 * to identify an argument on the command line.  Note that the
		 * _name does NOT include the two dashes as part of the _name. The
		 * _name cannot be blank.
		 */
		StringType _name;

		/**
		 * Description of the argument.
		 */
		StringType _description;

		/**
		 * Indicating whether the argument is required.
		 */
		bool _required;

		/**
		 * Label to be used in usage description.  Normally set to
		 * "required", but can be changed when necessary.
		 */
		StringType _requireLabel;

		/**
		 * Indicates whether a value is required for the argument.
		 * Note that the value may be required but the argument/value
		 * combination may not be, as specified by _required.
		 */
		bool _valueRequired;

		/**
		 * Indicates whether the argument has been set.
		 * Indicates that a value on the command line has matched the
		 * name/flag of this argument and the values have been set accordingly.
		 */
		bool _alreadySet;

		/**
		 * A pointer to a visitor object.
		 * The visitor allows special handling to occur as soon as the
		 * argument is matched.  This defaults to NULL and should not
		 * be used unless absolutely necessary.
		 */
		Visitor* _visitor;

		/**
		 * Whether this argument can be ignored, if desired.
		 */
		bool _ignoreable;

		/**
		 * Indicates that the arg was set as part of an XOR and not on the
		 * command line.
		 */
		bool _xorSet;

		bool _acceptsMultipleValues;

		/**
		 * Performs the special handling described by the Visitor.
		 */
		void _checkWithVisitor() const;

		/**
		 * Primary constructor. YOU (yes you) should NEVER construct an Arg
		 * directly, this is a base class that is extended by various children
		 * that are meant to be used.  Use SwitchArg, ValueArg, MultiArg,
		 * UnlabeledValueArg, or UnlabeledMultiArg instead.
		 *
		 * \param flag - The flag identifying the argument.
		 * \param name - The name identifying the argument.
		 * \param desc - The description of the argument, used in the usage.
		 * \param req - Whether the argument is required.
		 * \param valreq - Whether the a value is required for the argument.
		 * \param v - The visitor checked by the argument. Defaults to NULL.
		 */
		Arg(const StringType& flag,
			const StringType& name,
			const StringType& desc,
			bool req,
			bool valreq,
			Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType());

	private:
		/**
		 * Indicates whether the rest of the arguments should be ignored.
		 */
		static bool& ignoreRestRef() { static bool ign = false; return ign; }

		/**
		 * The delimiter that separates an argument flag/name from the
		 * value.
		 */
		static CharType& delimiterRef() { static CharType delim = StringConvertType::fromConstBasicChar(' '); return delim; }
	};

	/**
	 * Typedef of an Arg list iterator.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	using ArgListIterator = typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgListIteratorType;

	/**
	 * Typedef of an Arg vector iterator.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	using ArgVectorIterator = typename Arg<T_Char, T_CharTraits, T_Alloc>::ArgVectorIteratorType;

	/**
	 * Typedef of a Visitor list iterator.
	 */
	template<typename T_Alloc = std::allocator<Visitor*>>
	using VisitorListIterator = typename std::list<Visitor*, T_Alloc>::const_iterator;

	/*
	 * Extract a value of type T from it's string representation contained
	 * in strVal. The ValueLike parameter used to select the correct
	 * specialization of ExtractValue depending on the value traits of T.
	 * ValueLike traits use operator>> to assign the value from strVal.
	 */
	template<typename T, typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>> void
	ExtractValue(T& destVal, const std::basic_string<T_Char, T_CharTraits, T_Alloc>& strVal, ValueLike vl) {
		static_cast<void>(vl); // Avoid warning about unused vl
		using StringConvertType = StringConvert<T_Char, T_CharTraits>;
		std::basic_istringstream<T_Char, T_CharTraits, T_Alloc> is(strVal.c_str());

		int valuesRead = 0;
		while (is.good()) {
			if (is.peek() != EOF)
#ifdef TCLAP_SETBASE_ZERO
				is >> std::setbase(0) >> destVal;
#else
				is >> destVal;
#endif
			else
				break;

			valuesRead++;
		}

		if (is.fail())
			throw(ArgParseException<T_Char, T_CharTraits, T_Alloc>(StringConvertType::fromConstBasicCharString("Couldn\'t read argument value from string \'") + strVal + StringConvertType::fromConstBasicCharString("\'")));


		if (valuesRead > 1)
			throw(ArgParseException<T_Char, T_CharTraits, T_Alloc>(StringConvertType::fromConstBasicCharString("More than one valid value parsed from string \'") + strVal + StringConvertType::fromConstBasicCharString("\'")));

	}

	/*
	 * Extract a value of type T from it's string representation contained
	 * in strVal. The ValueLike parameter used to select the correct
	 * specialization of ExtractValue depending on the value traits of T.
	 * StringLike uses assignment (operator=) to assign from strVal.
	 */
	template<typename T, typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>> void
		ExtractValue(T& destVal, const std::basic_string<T_Char, T_CharTraits, T_Alloc>& strVal, StringLike sl) {
		static_cast<void>(sl); // Avoid warning about unused sl
		SetString(destVal, strVal);
	}

	//////////////////////////////////////////////////////////////////////
	//BEGIN Arg.cpp
	//////////////////////////////////////////////////////////////////////

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline Arg<T_Char, T_CharTraits, T_Alloc>::Arg(const StringType& flag,
		const StringType& name,
		const StringType& desc,
		bool req,
		bool valreq,
		Visitor* v,
		const AllocatorType& alloc) :
		UseAllocatorBase<T_Alloc>(alloc),
		_flag(flag),
		_name(name),
		_description(desc),
		_required(req),
		_requireLabel(StringConvertType::fromConstBasicCharString("required")),
		_valueRequired(valreq),
		_alreadySet(false),
		_visitor(v),
		_ignoreable(true),
		_xorSet(false),
		_acceptsMultipleValues(false) {
		if (_flag.length() > 1)
			throw(SpecificationException<T_Char, T_CharTraits, T_Alloc>("Argument flag can only be one character long", toString()));

		if (_name != ignoreNameString() &&
			(_flag == Arg::flagStartString() ||
				_flag == Arg::nameStartString() ||
				_flag == " "))
			throw(SpecificationException<T_Char, T_CharTraits, T_Alloc>(
				StringConvertType::fromConstBasicCharString("Argument flag cannot be either '") +
				Arg::flagStartString() + StringConvertType::fromConstBasicCharString("' or '") +
				Arg::nameStartString() + StringConvertType::fromConstBasicCharString("' or a space."),
				toString()
			));

		if ((_name.substr(0, Arg::flagStartString().length()) == Arg::flagStartString()) ||
			(_name.substr(0, Arg::nameStartString().length()) == Arg::nameStartString()) ||
			(_name.find(" ", 0) != StringType::npos))
			throw(SpecificationException<T_Char, T_CharTraits, T_Alloc>(
				StringConvertType::fromConstBasicCharString("Argument name begin with either '") +
				Arg::flagStartString() + StringConvertType::fromConstBasicCharString("' or '") +
				Arg::nameStartString() + StringConvertType::fromConstBasicCharString("' or space."),
				toString()
			));

	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline Arg<T_Char, T_CharTraits, T_Alloc>::~Arg() {}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto Arg<T_Char, T_CharTraits, T_Alloc>::shortID(const StringType& valueId) const -> StringType {
		StringType id;

		if (!_flag.empty())
			id = Arg::flagStartString() + _flag;
		else
			id = Arg::nameStartString() + _name;

		if (_valueRequired)
			id += StringType(1, Arg::delimiter()) + StringConvertType::fromConstBasicCharString("<") + valueId + StringConvertType::fromConstBasicCharString(">");

		if (!_required)
			id = StringConvertType::fromConstBasicCharString("[") + id + StringConvertType::fromConstBasicCharString("]");

		return id;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto Arg<T_Char, T_CharTraits, T_Alloc>::longID(const StringType& valueId) const -> StringType {
		StringType id;

		if (!_flag.empty()) 	{
			id += Arg::flagStartString() + _flag;

			if (_valueRequired)
				id += StringType(1, Arg::delimiter()) + StringConvertType::fromConstBasicCharString("<") + valueId + StringConvertType::fromConstBasicCharString(">");

			id += StringConvertType::fromConstBasicCharString(",  ");
		}

		id += Arg::nameStartString() + _name;

		if (_valueRequired)
			id += StringType(1, Arg::delimiter()) + StringConvertType::fromConstBasicCharString("<") + valueId + StringConvertType::fromConstBasicCharString(">");

		return id;

	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::operator==(const ArgType& a) const {
		if ((!_flag.empty() && _flag == a._flag) || _name == a._name)
			return true;
		else
			return false;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto Arg<T_Char, T_CharTraits, T_Alloc>::getDescription() const -> StringType {
		StringType desc;
		if (_required)
			desc = StringConvertType::fromConstBasicCharString("(") + _requireLabel + StringConvertType::fromConstBasicCharString(")  ");

		//	if ( _valueRequired )
		//		desc += StringConvertType::fromConstBasicCharString("(value required)  ");

		desc += _description;
		return desc;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto Arg<T_Char, T_CharTraits, T_Alloc>::getFlag() const -> const StringType& { return _flag; }

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto Arg<T_Char, T_CharTraits, T_Alloc>::getName() const -> const StringType& { return _name; }

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::isRequired() const { return _required; }

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::isValueRequired() const { return _valueRequired; }

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::isSet() const {
		if (_alreadySet && !_xorSet)
			return true;
		else
			return false;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::isIgnoreable() const { return _ignoreable; }

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void Arg<T_Char, T_CharTraits, T_Alloc>::setRequireLabel(const StringType& s) {
		_requireLabel = s;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::argMatches(const StringType& argFlag) const {
		if ((argFlag == Arg::flagStartString() + _flag && !_flag.empty()) ||
			argFlag == Arg::nameStartString() + _name)
			return true;
		else
			return false;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto Arg<T_Char, T_CharTraits, T_Alloc>::toString() const -> StringType {
		StringType s;

		if (!_flag.empty())
			s += Arg::flagStartString() + _flag + StringConvertType::fromConstBasicCharString(" ");

		s += StringConvertType::fromConstBasicCharString("(") + Arg::nameStartString() + _name + StringConvertType::fromConstBasicCharString(")");

		return s;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void Arg<T_Char, T_CharTraits, T_Alloc>::_checkWithVisitor() const {
		if (_visitor != NULL)
			_visitor->visit();
	}

	/**
	 * Implementation of trimFlag.
	 */
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void Arg<T_Char, T_CharTraits, T_Alloc>::trimFlag(StringType& flag, StringType& value) const {
		std::size_t stop = 0;
		for (std::size_t i = 0; i < flag.length(); i++)
			if (flag[i] == Arg::delimiter()) 		{
				stop = i;
				break;
			}

		if (stop > 1) 	{
			value = flag.substr(stop + 1);
			flag = flag.substr(0, stop);
		}

	}

	/**
	 * Implementation of _hasBlanks.
	 */
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::_hasBlanks(const StringType& s) const {
		for (int i = 1; static_cast<unsigned int>(i) < s.length(); i++)
			if (s[i] == Arg::blankChar())
				return true;

		return false;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void Arg<T_Char, T_CharTraits, T_Alloc>::forceRequired() {
		_required = true;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void Arg<T_Char, T_CharTraits, T_Alloc>::xorSet() {
		_alreadySet = true;
		_xorSet = true;
	}

	/**
	 * Overridden by Args that need to added to the end of the list.
	 */
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void Arg<T_Char, T_CharTraits, T_Alloc>::addToList(ArgListType& argList) const {
		argList.push_front(const_cast<ArgType*>(this));
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::allowMore() {
		return false;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool Arg<T_Char, T_CharTraits, T_Alloc>::acceptsMultipleValues() {
		return _acceptsMultipleValues;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void Arg<T_Char, T_CharTraits, T_Alloc>::reset() {
		_xorSet = false;
		_alreadySet = false;
	}

	//////////////////////////////////////////////////////////////////////
	//END Arg.cpp
	//////////////////////////////////////////////////////////////////////

} //namespace TCLAP

#endif

