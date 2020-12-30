// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-



/******************************************************************************
 *
 *  file:  ValuesConstraint.h
 *
 *  Copyright (c) 2005, Michael E. Smoot
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

#ifndef TCLAP_VALUESCONSTRAINT_H
#define TCLAP_VALUESCONSTRAINT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <utility>
#include <string>
#include <vector>
#include <sstream>

#include <tclap/StringConvert.h>
#include <tclap/Constraint.h>

namespace TCLAP {

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	class ValuesConstraint;

	/**
	 * A Constraint that constrains the Arg to only those values specified
	 * in the constraint.
	 */
	template<class T, typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class ValuesConstraint : public Constraint<T, T_Char, T_CharTraits, T_Alloc> {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using typename Constraint<T, T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename Constraint<T, T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename Constraint<T, T_Char, T_CharTraits, T_Alloc>::StringConvertType;
		using typename Constraint<T, T_Char, T_CharTraits, T_Alloc>::StringViewType;
		using typename Constraint<T, T_Char, T_CharTraits, T_Alloc>::StringType;
		using typename Constraint<T, T_Char, T_CharTraits, T_Alloc>::StringVectorType;
		using container_type = std::vector<T, typename std::allocator_traits<AllocatorType>::template rebind_alloc<T>>;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

		/**
		 * Constructor.
		 * \param allowed - vector of allowed values.
		 */
		ValuesConstraint(const container_type& allowed, const AllocatorType& alloc = AllocatorType());

		/**
		 * Virtual destructor.
		 */
		virtual ~ValuesConstraint() {}

		/**
		 * Returns a description of the Constraint.
		 */
		virtual StringType description() const;

		/**
		 * Returns the short ID for the Constraint.
		 */
		virtual StringType shortID() const;

		/**
		 * The method used to verify that the value parsed from the command
		 * line meets the constraint.
		 * \param value - The value that will be checked.
		 */
		virtual bool check(const T& value) const;

	protected:

		/**
		 * The list of valid values.
		 */
		container_type _allowed;

		/**
		 * The string used to describe the allowed values of this constraint.
		 */
		StringType _typeDesc;

	};

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	ValuesConstraint<T, T_Char, T_CharTraits, T_Alloc>::ValuesConstraint(const container_type& allowed, const AllocatorType& alloc)
		: Constraint<T, T_Char, T_CharTraits, T_Alloc>(alloc), _allowed(allowed) {
		bool is_first_value = true;
		for (T& value : _allowed) {
			if (is_first_value)
				is_first_value = false;
			else
				_typeDesc += StringConvertType::fromConstBasicCharString("|");
			{
				std::basic_ostringstream<T_Char, T_CharTraits, T_Alloc> os;
				os << value;
				_typeDesc += os.str();
			}
		}
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	bool ValuesConstraint<T, T_Char, T_CharTraits, T_Alloc>::check(const T& val) const {
		if (std::find(_allowed.begin(), _allowed.end(), val) == _allowed.end())
			return false;
		else
			return true;
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	auto ValuesConstraint<T, T_Char, T_CharTraits, T_Alloc>::shortID() const -> StringType {
		return _typeDesc;
	}

	template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
	auto ValuesConstraint<T, T_Char, T_CharTraits, T_Alloc>::description() const -> StringType {
		return _typeDesc;
	}


} //namespace TCLAP
#endif 

