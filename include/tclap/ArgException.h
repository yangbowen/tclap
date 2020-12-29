// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  ArgException.h
 *
 *  Copyright (c) 2003, Michael E. Smoot .
 *  Copyright (c) 2017 Google LLC
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


#ifndef TCLAP_ARG_EXCEPTION_H
#define TCLAP_ARG_EXCEPTION_H

#include <string>
#include <exception>

#include <tclap/StringConvert.h>
#include <tclap/UseAllocatorBase.h>

namespace TCLAP {
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class ArgException;
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class ArgParseException;
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class CmdLineParseException;
	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	class SpecificationException;

	/**
	 * A simple class that defines and argument exception.  Should be caught
	 * whenever a CmdLine is created and parsed.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class ArgException : public UseAllocatorBase<T_Alloc>, public std::exception {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using StringConvertType = StringConvert<T_Char, T_CharTraits>;
		using StringType = std::basic_string<T_Char, T_CharTraits, T_Alloc>;

		/**
		 * Constructor.
		 * \param text - The text of the exception.
		 * \param id - The text identifying the argument source.
		 * \param td - Text describing the type of ArgException it is.
		 * of the exception.
		 */
		ArgException(const StringType& text = StringConvertType::fromConstBasicCharString("undefined exception"), const StringType& id = StringType())
			: ArgException(text, id, StringConvertType::fromConstBasicCharString("Generic ArgException")) {
		}

		/**
		 * Destructor.
		 */
		virtual ~ArgException() noexcept override {}

		ArgException(const ArgException& rhs) = delete;
		ArgException& operator=(const ArgException& rhs) = delete;

		/**
		 * Returns the error text.
		 */
		StringType error() const { return _errorText; }

		/**
		 * Returns the argument id.
		 */
		StringType argId() const {
			if (_argId.empty())
				return StringConvertType::fromConstBasicCharString(" ");
			else
				return StringConvertType::fromConstBasicCharString("Argument: ") + _argId;
		}

		/**
		 * Returns the type of the exception.  Used to explain and distinguish
		 * between different child exceptions.
		 */
		StringType typeDescription() const {
			return _typeDescription;
		}

	protected:
		/**
		 * Constructor.
		 * \param text - The text of the exception.
		 * \param id - The text identifying the argument source.
		 * \param td - Text describing the type of ArgException it is.
		 * of the exception.
		 */
		ArgException(const StringType& text, const StringType& id, const StringType& td)
			: UseAllocatorBase<T_Alloc>(text.get_allocator()),
			std::exception(StringConvertType::template toMBString<T_Alloc>(id + StringConvertType::fromConstBasicCharString(" -- ") + text).c_str()),
			_errorText(text),
			_argId(id),
			_typeDescription(td) {
		}

	private:
		/**
		 * The text of the exception message.
		 */
		StringType _errorText;

		/**
		 * The argument related to this exception.
		 */
		StringType _argId;

		/**
		 * Describes the type of the exception.  Used to distinguish
		 * between different child exceptions.
		 */
		StringType _typeDescription;
	};

	/**
	 * Thrown from within the child Arg classes when it fails to properly
	 * parse the argument it has been passed.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class ArgParseException : public ArgException<T_Char, T_CharTraits, T_Alloc> {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::StringConvertType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::StringType;

		/**
		 * Constructor.
		 * \param text - The text of the exception.
		 * \param id - The text identifying the argument source
		 * of the exception.
		 */
		ArgParseException(const StringType& text = StringConvertType::fromConstBasicCharString("undefined exception"), const StringType& id = StringType())
			: ArgException<T_Char, T_CharTraits, T_Alloc>(text, id, StringConvertType::fromConstBasicCharString("Exception found while parsing the value the Arg has been passed.")) {
		}
	};

	/**
	 * Thrown from CmdLine when the arguments on the command line are not
	 * properly specified, e.g. too many arguments, required argument missing, etc.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class CmdLineParseException : public ArgException<T_Char, T_CharTraits, T_Alloc> {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::StringConvertType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::StringType;

		/**
		 * Constructor.
		 * \param text - The text of the exception.
		 * \param id - The text identifying the argument source
		 * of the exception.
		 */
		CmdLineParseException(const StringType& text = StringConvertType::fromConstBasicCharString("undefined exception"), const StringType& id = StringType())
			: ArgException<T_Char, T_CharTraits, T_Alloc>(text, id, StringConvertType::fromConstBasicCharString("Exception found when the values on the command line do not meet the requirements of the defined Args.")) {
		}
	};

	/**
	 * Thrown from Arg and CmdLine when an Arg is improperly specified, e.g.
	 * same flag as another Arg, same name, etc.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class SpecificationException : public ArgException<T_Char, T_CharTraits, T_Alloc> {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::StringConvertType;
		using typename ArgException<T_Char, T_CharTraits, T_Alloc>::StringType;

		/**
		 * Constructor.
		 * \param text - The text of the exception.
		 * \param id - The text identifying the argument source
		 * of the exception.
		 */
		SpecificationException(const StringType& text = StringConvertType::fromConstBasicCharString("undefined exception"), const StringType& id = StringType())
			: ArgException<T_Char, T_CharTraits, T_Alloc>(text, id, StringConvertType::fromConstBasicCharString("Exception found when an Arg object is improperly defined by the developer.")) {
		}

	};

	class ExitException {
	public:
		ExitException(int estat) : _estat(estat) {}

		int getExitStatus() const { return _estat; }

	private:
		int _estat;
	};

} // namespace TCLAP

#endif

