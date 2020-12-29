// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  CmdLine.h
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

#ifndef TCLAP_CMDLINE_H
#define TCLAP_CMDLINE_H

#include <tclap/StringConvert.h>

#include <tclap/SwitchArg.h>
#include <tclap/MultiSwitchArg.h>
#include <tclap/UnlabeledValueArg.h>
#include <tclap/UnlabeledMultiArg.h>

#include <tclap/XorHandler.h>
#include <tclap/Visitor.h>
#include <tclap/HelpVisitor.h>
#include <tclap/VersionVisitor.h>
#include <tclap/IgnoreRestVisitor.h>

#include <tclap/CmdLineOutput.h>
#include <tclap/StdOutput.h>

#include <tclap/Constraint.h>
#include <tclap/ValuesConstraint.h>

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdlib.h> // Needed for exit(), which isn't defined in some envs.

namespace TCLAP {
	/**
	 * The base class that manages the command line definition and passes
	 * along the parsing to the appropriate Arg classes.
	 */
	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
	class CmdLine : public CmdLineInterface<T_Char, T_CharTraits, T_Alloc> {
	public:
		using typename UseAllocatorBase<T_Alloc>::AllocatorType;
		using typename UseAllocatorBase<T_Alloc>::AllocatorTraitsType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::CharType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::CharTraitsType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::StringConvertType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::StringType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::StringVectorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgListType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgVectorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgListIteratorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::ArgVectorIteratorType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::CmdLineInterfaceType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::CmdLineOutputType;
		using typename CmdLineInterface<T_Char, T_CharTraits, T_Alloc>::XorHandlerType;
		using VisitorListType = std::list<Visitor*, typename std::allocator_traits<AllocatorType>::template rebind_alloc<Visitor*>>;
		using UseAllocatorBase<T_Alloc>::getAlloc;
		using UseAllocatorBase<T_Alloc>::rebindAlloc;

	protected:

		/**
		 * The list of arguments that will be tested against the
		 * command line.
		 */
		ArgListType _argList;

		/**
		 * The name of the program.  Set to argv[0].
		 */
		StringType _progName;

		/**
		 * A message used to describe the program.  Used in the usage output.
		 */
		StringType _message;

		/**
		 * The version to be displayed with the --version switch.
		 */
		StringType _version;

		/**
		 * The number of arguments that are required to be present on
		 * the command line. This is set dynamically, based on the
		 * Args added to the CmdLine object.
		 */
		std::size_t _numRequired;

		/**
		 * The character that is used to separate the argument flag/name
		 * from the value.  Defaults to ' ' (space).
		 */
		CharType _delimiter;

		/**
		 * The handler that manages xoring lists of args.
		 */
		XorHandlerType _xorHandler;

		/**
		 * A list of Args to be explicitly deleted when the destructor
		 * is called.  At the moment, this only includes the three default
		 * Args.
		 */
		std::vector<std::unique_ptr<ArgType>, typename std::allocator_traits<AllocatorType>::template rebind_alloc<std::unique_ptr<ArgType>>> _argPrivateVec;

		/**
		 * A list of Visitors to be explicitly deleted when the destructor
		 * is called.  At the moment, these are the Visitors created for the
		 * default Args.
		 */
		std::vector<std::unique_ptr<Visitor>, typename std::allocator_traits<AllocatorType>::template rebind_alloc<std::unique_ptr<Visitor>>> _visitorPrivateVec;

		/**
		 * Object that handles all output for the CmdLine.
		 */
		CmdLineOutputType* _output = nullptr;

		std::unique_ptr<CmdLineOutputType> _outputDefault;

		/**
		 * Should CmdLine handle parsing exceptions internally?
		 */
		bool _handleExceptions;

		/**
		 * Throws an exception listing the missing args.
		 */
		void missingArgsException();

		/**
		 * Checks whether a name/flag string matches entirely matches
		 * the ArgType::blankChar.  Used when multiple switches are combined
		 * into a single argument.
		 * \param s - The message to be used in the usage.
		 */
		bool _emptyCombined(const StringType& s);
	private:

		/**
		 * Prevent accidental copying.
		 */
		CmdLine(const CmdLine& rhs);
		CmdLine& operator=(const CmdLine& rhs);

		/**
		 * Encapsulates the code common to the constructors
		 * (which is all of it).
		 */
		void _constructor();

		/**
		 * Whether or not to automatically create help and version switches.
		 */
		bool _helpAndVersion;

		/**
		 * Whether or not to ignore unmatched args.
		 */
		bool _ignoreUnmatched;

	public:

		/**
		 * Command line constructor. Defines how the arguments will be
		 * parsed.
		 * \param message - The message to be used in the usage
		 * output.
		 * \param delimiter - The character that is used to separate
		 * the argument flag/name from the value.  Defaults to ' ' (space).
		 * \param version - The version number to be used in the
		 * --version switch.
		 * \param helpAndVersion - Whether or not to create the Help and
		 * Version switches. Defaults to true.
		 */
		CmdLine(const StringType& message,
			const CharType delimiter = StringConvertType::fromConstBasicChar(' '),
			const StringType& version = StringConvertType::fromConstBasicCharString("none"),
			bool helpAndVersion = true,
			const AllocatorType& alloc = AllocatorType());

		/**
		 * Deletes any resources allocated by a CmdLine object.
		 */
		virtual ~CmdLine() override;

		/**
		 * Adds an argument to the list of arguments to be parsed.
		 * \param a - Argument to be added.
		 */
		virtual void add(ArgType& a) override;

		/**
		 * An alternative add.  Functionally identical.
		 * \param a - Argument to be added.
		 */
		virtual void add(ArgType* a) override;

		/**
		 * Add two Args that will be xor'd.  If this method is used, add does
		 * not need to be called.
		 * \param a - Argument to be added and xor'd.
		 * \param b - Argument to be added and xor'd.
		 */
		virtual void xorAdd(ArgType& a, ArgType& b) override;

		/**
		 * Add a list of Args that will be xor'd.  If this method is used,
		 * add does not need to be called.
		 * \param xors - List of Args to be added and xor'd.
		 */
		virtual void xorAdd(const ArgVectorType& xors) override;

		/**
		 * Parses the command line.
		 * \param argc - Number of arguments.
		 * \param argv - Array of arguments.
		 */
		virtual void parse(int argc, const CharType* const* argv) override;

		/**
		 * Parses the command line.
		 * \param args - A vector of strings representing the args.
		 * args[0] is still the program name.
		 */
		virtual void parse(StringVectorType& args) override;

		/**
		 *
		 */
		virtual CmdLineOutputType* getOutput() override;

		/**
		 *
		 */
		virtual void setOutput(CmdLineOutputType* co) override;

		/**
		 *
		 */
		virtual StringType& getVersion() override;

		/**
		 *
		 */
		virtual StringType& getProgramName() override;

		/**
		 *
		 */
		virtual ArgListType& getArgList() override;

		/**
		 *
		 */
		virtual XorHandlerType& getXorHandler() override;

		/**
		 *
		 */
		virtual CharType getDelimiter() override;

		/**
		 *
		 */
		virtual StringType& getMessage() override;

		/**
		 *
		 */
		virtual bool hasHelpAndVersion() override;

		/**
		 * Disables or enables CmdLine's internal parsing exception handling.
		 *
		 * @param state Should CmdLine handle parsing exceptions internally?
		 */
		void setExceptionHandling(const bool state);

		/**
		 * Returns the current state of the internal exception handling.
		 *
		 * @retval true Parsing exceptions are handled internally.
		 * @retval false Parsing exceptions are propagated to the caller.
		 */
		bool getExceptionHandling() const;

		/**
		 * Allows the CmdLine object to be reused.
		 */
		virtual void reset() override;

		/**
		 * Allows unmatched args to be ignored. By default false.
		 *
		 * @param ignore If true the cmdline will ignore any unmatched args
		 * and if false it will behave as normal.
		 */
		void ignoreUnmatched(const bool ignore);
	};


	///////////////////////////////////////////////////////////////////////////////
	//Begin CmdLine.cpp
	///////////////////////////////////////////////////////////////////////////////

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline CmdLine<T_Char, T_CharTraits, T_Alloc>::CmdLine(const StringType& m,
		CharType delim,
		const StringType& v,
		bool help,
		const AllocatorType& alloc)
		:
		CmdLineInterface<T_Char, T_CharTraits, T_Alloc>(alloc),
		_argList(ArgListType()),
		_progName(StringConvertType::fromConstBasicCharString("not_set_yet")),
		_message(m),
		_version(v),
		_numRequired(0),
		_delimiter(delim),
		_xorHandler(XorHandlerType()),
		_argPrivateVec(alloc),
		_visitorPrivateVec(alloc),
		_handleExceptions(true),
		_helpAndVersion(help),
		_ignoreUnmatched(false) {
		_constructor();
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline CmdLine<T_Char, T_CharTraits, T_Alloc>::~CmdLine() {}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::_constructor() {
		_outputDefault = std::make_unique<StdOutput<T_Char, T_CharTraits, T_Alloc>>();
		_output = _outputDefault.get();

		ArgType::setDelimiter(_delimiter);

		if (_helpAndVersion) 	{
			{
				std::unique_ptr<HelpVisitor<T_Char, T_CharTraits, T_Alloc>> v = std::make_unique<HelpVisitor<T_Char, T_CharTraits, T_Alloc>>(this, &_output, getAlloc());
				std::unique_ptr<SwitchArg<T_Char, T_CharTraits, T_Alloc>> help = std::make_unique<SwitchArg<T_Char, T_CharTraits, T_Alloc>>(
					StringConvertType::fromConstBasicCharString("h"),
					StringConvertType::fromConstBasicCharString("help"),
					StringConvertType::fromConstBasicCharString("Displays usage information and exits."),
					false, &*v, getAlloc());
				add(*help);
				_visitorPrivateVec.push_back(std::move(v));
				_argPrivateVec.push_back(std::move(help));
			}
			{
				std::unique_ptr<VersionVisitor<T_Char, T_CharTraits, T_Alloc>> v = std::make_unique<VersionVisitor<T_Char, T_CharTraits, T_Alloc>>(this, &_output, getAlloc());
				std::unique_ptr<SwitchArg<T_Char, T_CharTraits, T_Alloc>> vers = std::make_unique<SwitchArg<T_Char, T_CharTraits, T_Alloc>>(
					StringType(),
					StringConvertType::fromConstBasicCharString("version"),
					StringConvertType::fromConstBasicCharString("Displays version information and exits."),
					false, &*v, getAlloc());
				add(*vers);
				_visitorPrivateVec.push_back(std::move(v));
				_argPrivateVec.push_back(std::move(vers));
			}
		}
		{
			std::unique_ptr<IgnoreRestVisitor<T_Char, T_CharTraits, T_Alloc>> v = std::make_unique<IgnoreRestVisitor<T_Char, T_CharTraits, T_Alloc>>(getAlloc());
			std::unique_ptr<SwitchArg<T_Char, T_CharTraits, T_Alloc>> ignore = std::make_unique<SwitchArg<T_Char, T_CharTraits, T_Alloc>>(ArgType::flagStartString(),
				ArgType::ignoreNameString(),
				StringConvertType::fromConstBasicCharString("Ignores the rest of the labeled arguments following this flag."),
				false, &*v, getAlloc());
			add(*ignore);
			_visitorPrivateVec.push_back(std::move(v));
			_argPrivateVec.push_back(std::move(ignore));
		}
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::xorAdd(const ArgVectorType& ors) {
		_xorHandler.add(ors);

		for (ArgType* const& arg : ors) 	{
			arg->forceRequired();
			arg->setRequireLabel(StringConvertType::fromConstBasicCharString("OR required"));
			add(arg);
		}
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::xorAdd(ArgType& a, ArgType& b) {
		ArgVectorType ors;
		ors.push_back(&a);
		ors.push_back(&b);
		xorAdd(ors);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::add(ArgType& a) {
		add(&a);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::add(ArgType* a) {
		for (const ArgType* const& arg : _argList)
			if (*a == *arg)
				throw(SpecificationException<T_Char, T_CharTraits, T_Alloc>(
					StringConvertType::fromConstBasicCharString("Argument with same flag/name already exists!"),
					a->longID()
				));

		a->addToList(_argList);

		if (a->isRequired())
			_numRequired++;
	}


	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::parse(int argc, const CharType* const* argv) {
		// this step is necessary so that we have easy access to
		// mutable strings.
		StringVectorType args;
		for (int i = 0; i < argc; i++) args.push_back(argv[i]);
		parse(args);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::parse(StringVectorType& args) {
		bool shouldExit = false;
		int estat = 0;

		try {
			_progName = args.front();
			args.erase(args.begin());

			std::size_t requiredCount = 0;

			for (std::size_t idx_arg = 0; idx_arg < args.size(); idx_arg++) 		{
				bool matched = false;
				for (ArgType* const& arg : _argList) {
					if (arg->processArg(idx_arg, args)) 				{
						requiredCount += _xorHandler.check(arg);
						matched = true;
						break;
					}
				}

				// checks to see if the argument is an empty combined
				// switch and if so, then we've actually matched it
				if (!matched && _emptyCombined(args[idx_arg]))
					matched = true;

				if (!matched && !ArgType::ignoreRest() && !_ignoreUnmatched)
					throw(CmdLineParseException<T_Char, T_CharTraits, T_Alloc>(StringConvertType::fromConstBasicCharString("Couldn't find match for argument"), args[idx_arg]));
			}

			if (requiredCount < _numRequired)
				missingArgsException();

			if (requiredCount > _numRequired)
				throw(CmdLineParseException<T_Char, T_CharTraits, T_Alloc>(StringConvertType::fromConstBasicCharString("Too many arguments!")));

		} catch (ArgException<T_Char, T_CharTraits, T_Alloc>& e) {
			// If we're not handling the exceptions, rethrow.
			if (!_handleExceptions) {
				throw;
			}

			try {
				_output->failure(*this, e);
			} catch (ExitException& ee) {
				estat = ee.getExitStatus();
				shouldExit = true;
			}
		} catch (ExitException& ee) {
			// If we're not handling the exceptions, rethrow.
			if (!_handleExceptions) {
				throw;
			}

			estat = ee.getExitStatus();
			shouldExit = true;
		}

		if (shouldExit)
			exit(estat);
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool CmdLine<T_Char, T_CharTraits, T_Alloc>::_emptyCombined(const StringType& s) {
		if (!s.empty() && s.front() != ArgType::flagStartChar()) return false;
		for (const CharType& ch : s) if (ch != ArgType::blankChar()) return false;
		return true;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::missingArgsException() {
		std::size_t count = 0;
		bool is_first_missing_arg = true;
		StringType missingArgList;
		for (const ArgType* const& arg : _argList) {
			if (arg->isRequired() && !arg->isSet()) {
				if (is_first_missing_arg)
					is_first_missing_arg = false;
				else
					missingArgList += StringConvertType::fromConstBasicCharString(", ");
				missingArgList += arg->getName();
				count++;
			}
		}
		StringType msg;
		if (count > 1)
			msg = StringConvertType::fromConstBasicCharString("Required arguments missing: ") + missingArgList;
		else
			msg = StringConvertType::fromConstBasicCharString("Required argument missing: ") + missingArgList;
		throw(CmdLineParseException<T_Char, T_CharTraits, T_Alloc>(msg));
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getOutput() -> CmdLineOutputType* {
		return _output;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::setOutput(CmdLineOutputType* co) {
		_output = co;
		_outputDefault.reset();
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getVersion() -> StringType& {
		return _version;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getProgramName() -> StringType& {
		return _progName;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getArgList() -> ArgListType& {
		return _argList;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getXorHandler() -> XorHandlerType& {
		return _xorHandler;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getDelimiter() -> CharType {
		return _delimiter;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline auto CmdLine<T_Char, T_CharTraits, T_Alloc>::getMessage() -> StringType& {
		return _message;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool CmdLine<T_Char, T_CharTraits, T_Alloc>::hasHelpAndVersion() {
		return _helpAndVersion;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::setExceptionHandling(const bool state) {
		_handleExceptions = state;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline bool CmdLine<T_Char, T_CharTraits, T_Alloc>::getExceptionHandling() const {
		return _handleExceptions;
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::reset() {
		for (ArgType* const& arg : _argList) arg->reset();
		_progName.clear();
	}

	template<typename T_Char, typename T_CharTraits, typename T_Alloc>
	inline void CmdLine<T_Char, T_CharTraits, T_Alloc>::ignoreUnmatched(const bool ignore) {
		_ignoreUnmatched = ignore;
	}

	///////////////////////////////////////////////////////////////////////////////
	//End CmdLine.cpp
	///////////////////////////////////////////////////////////////////////////////



} //namespace TCLAP
#endif
