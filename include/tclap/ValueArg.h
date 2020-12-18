// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/****************************************************************************** 
 * 
 *  file:  ValueArg.h
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


#ifndef TCLAP_VALUE_ARGUMENT_H
#define TCLAP_VALUE_ARGUMENT_H

#include <string>
#include <vector>

#include <tclap/Arg.h>
#include <tclap/Constraint.h>

namespace TCLAP {

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
class Constraint;
template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
class ValueArg;

/**
 * The basic labeled argument that parses a value.
 * This is a template class, which means the type T defines the type
 * that a given object will attempt to parse when the flag/name is matched
 * on the command line.  While there is nothing stopping you from creating
 * an unflagged ValueArg, it is unwise and would cause significant problems.
 * Instead use an UnlabeledValueArg.
 */
template<class T, typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class ValueArg : public Arg<T_Char, T_CharTraits, T_Alloc>
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
    using ConstraintType = Constraint<T, T_Char, T_CharTraits, T_Alloc>;
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

  /**
   * Labeled ValueArg constructor.
   * You could conceivably call this constructor with a blank flag, 
   * but that would make you a bad person.  It would also cause
   * an exception to be thrown.   If you want an unlabeled argument, 
   * use the other constructor.
   * \param flag - The one character flag that identifies this
   * argument on the command line.
   * \param name - A one word name for the argument.  Can be
   * used as a long flag on the command line.
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
   * \param v - An optional visitor.  You probably should not
   * use this unless you have a very good reason.
   */
  ValueArg( const StringType& flag, 
            const StringType& name, 
            const StringType& desc, 
            bool req, 
            T value,
            const StringType& typeDesc,
            Visitor* v = nullptr,
            const AllocatorType& alloc = AllocatorType() );
                                 
                                 
  /**
   * Labeled ValueArg constructor.
   * You could conceivably call this constructor with a blank flag, 
   * but that would make you a bad person.  It would also cause
   * an exception to be thrown.   If you want an unlabeled argument, 
   * use the other constructor.
   * \param flag - The one character flag that identifies this
   * argument on the command line.
   * \param name - A one word name for the argument.  Can be
   * used as a long flag on the command line.
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
   * \param v - An optional visitor.  You probably should not
   * use this unless you have a very good reason.
   */
  ValueArg( const StringType& flag, 
            const StringType& name, 
            const StringType& desc, 
            bool req, 
            T value,
            const StringType& typeDesc,
            CmdLineInterfaceType& parser,
            Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType() );
 
  /**
   * Labeled ValueArg constructor.
   * You could conceivably call this constructor with a blank flag, 
   * but that would make you a bad person.  It would also cause
   * an exception to be thrown.   If you want an unlabeled argument, 
   * use the other constructor.
   * \param flag - The one character flag that identifies this
   * argument on the command line.
   * \param name - A one word name for the argument.  Can be
   * used as a long flag on the command line.
   * \param desc - A description of what the argument is for or
   * does.
   * \param req - Whether the argument is required on the command
   * line.
   * \param value - The default value assigned to this argument if it
   * is not present on the command line.
   * \param constraint - A pointer to a Constraint object used
   * to constrain this Arg.
   * \param parser - A CmdLine parser object to add this Arg to.
   * \param v - An optional visitor.  You probably should not
   * use this unless you have a very good reason.
   */
  ValueArg( const StringType& flag, 
            const StringType& name, 
            const StringType& desc, 
            bool req, 
            T value,
            ConstraintType* constraint,
            CmdLineInterfaceType& parser,
            Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType() );
          
  /**
   * Labeled ValueArg constructor.
   * You could conceivably call this constructor with a blank flag, 
   * but that would make you a bad person.  It would also cause
   * an exception to be thrown.   If you want an unlabeled argument, 
   * use the other constructor.
   * \param flag - The one character flag that identifies this
   * argument on the command line.
   * \param name - A one word name for the argument.  Can be
   * used as a long flag on the command line.
   * \param desc - A description of what the argument is for or
   * does.
   * \param req - Whether the argument is required on the command
   * line.
   * \param value - The default value assigned to this argument if it
   * is not present on the command line.
   * \param constraint - A pointer to a Constraint object used
   * to constrain this Arg.
   * \param v - An optional visitor.  You probably should not
   * use this unless you have a very good reason.
   */
  ValueArg( const StringType& flag, 
            const StringType& name, 
            const StringType& desc, 
            bool req, 
            T value,
            ConstraintType* constraint,
            Visitor* v = nullptr,
			const AllocatorType& alloc = AllocatorType() );

  /**
   * Handles the processing of the argument.
   * This re-implements the Arg version of this method to set the
   * _value of the argument appropriately.  It knows the difference
   * between labeled and unlabeled.
   * \param i - Pointer the the current argument in the list.
   * \param args - Mutable list of strings. Passed 
   * in from main().
   */
  virtual bool processArg(int* i, StringVectorType& args); 

  /**
   * Returns the value of the argument.
   */
  const T& getValue() const { return _value; }

  // TODO(macbishop): Non-const variant is deprecated, don't
  // use. Remove in next major.
  T& getValue() { return _value; }

  /**
   * A ValueArg can be used as as its value type (T) This is the
   * same as calling getValue()
   */
  operator const T&() const { return getValue(); }

  /**
   * Specialization of shortID.
   * \param val - value to be used.
   */
  virtual StringType shortID(const StringType& val = "val") const;

  /**
   * Specialization of longID.
   * \param val - value to be used.
   */
  virtual StringType longID(const StringType& val = "val") const;
        
  virtual void reset() ;

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
   * The value parsed from the command line.
   * Can be of any type, as long as the >> operator for the type
   * is defined.
   */
  T _value;

  /**
   * Used to support the reset() method so that ValueArg can be
   * reset to their constructed value.
   */
  T _default;

  /**
   * A human readable description of the type to be parsed.
   * This is a hack, plain and simple.  Ideally we would use RTTI to
   * return the name of type T, but until there is some sort of
   * consistent support for human readable names, we are left to our
   * own devices.
   */
  StringType _typeDesc;

  /**
   * A Constraint this Arg must conform to. 
   */
  ConstraintType* _constraint;

  /**
   * Extracts the value from the string.
   * Attempts to parse string as type T, if this fails an exception
   * is thrown.
   * \param val - value to be parsed. 
   */
  void _extractValue( const StringType& val );

private:
  /**
   * Prevent accidental copying
   */
  ValueArg(const ValueArg& rhs);
  ValueArg& operator=(const ValueArg& rhs);
};


/**
 * Constructor implementation.
 */
template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
ValueArg<T, T_Char, T_CharTraits, T_Alloc>::ValueArg(const StringType& flag,
                      const StringType& name, 
                      const StringType& desc, 
                      bool req, 
                      T val,
                      const StringType& typeDesc,
                      Visitor* v,
			          const AllocatorType& alloc)
  : Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, req, true, v, alloc),
    _value( val ),
    _default( val ),
    _typeDesc( typeDesc ),
    _constraint( NULL )
{ }

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
ValueArg<T, T_Char, T_CharTraits, T_Alloc>::ValueArg(const StringType& flag,
                      const StringType& name, 
                      const StringType& desc, 
                      bool req, 
                      T val,
                      const StringType& typeDesc,
                      CmdLineInterfaceType& parser,
                      Visitor* v,
                      const AllocatorType& alloc)
  : Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, req, true, v, alloc),
    _value( val ),
    _default( val ),
    _typeDesc( typeDesc ),
    _constraint( NULL )
{ 
  parser.add( this );
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
ValueArg<T, T_Char, T_CharTraits, T_Alloc>::ValueArg(const StringType& flag,
                      const StringType& name, 
                      const StringType& desc, 
                      bool req, 
                      T val,
                      ConstraintType* constraint,
                      Visitor* v,
                      const AllocatorType& alloc)
  : Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, req, true, v, alloc),
    _value( val ),
    _default( val ),
    _typeDesc( ConstraintType::shortID(constraint) ),
    _constraint( constraint )
{ }

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
ValueArg<T, T_Char, T_CharTraits, T_Alloc>::ValueArg(const StringType& flag,
                      const StringType& name, 
                      const StringType& desc, 
                      bool req, 
                      T val,
                      ConstraintType* constraint,
                      CmdLineInterfaceType& parser,
                      Visitor* v,
                      const AllocatorType& alloc)
  : Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, req, true, v, alloc),
    _value( val ),
    _default( val ),
    _typeDesc( ConstraintType::shortID(constraint) ),  // TODO(macbishop): Will crash
    // if constraint is NULL
    _constraint( constraint )
{ 
  parser.add( this );
}

/**
 * Implementation of processArg().
 */
template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
bool ValueArg<T, T_Char, T_CharTraits, T_Alloc>::processArg(int *i, StringVectorType& args)
{
  if ( _ignoreable && Arg<T_Char, T_CharTraits, T_Alloc>::ignoreRest() )
    return false;

  if ( _hasBlanks( args[*i] ) )
    return false;

  StringType flag = args[*i];

  StringType value;
  trimFlag( flag, value );

  if ( argMatches( flag ) )
    {
      if ( _alreadySet )
        {
          if ( _xorSet )
            throw( CmdLineParseException("Mutually exclusive argument"
                                         " already set!", toString()));
          else
            throw( CmdLineParseException("Argument already set!", 
                                         toString()) );
        }

      if ( Arg<T_Char, T_CharTraits, T_Alloc>::delimiter() != ' ' && value.empty() )
        throw( ArgParseException("Couldn't find delimiter for this argument!",
                                 toString() ) );

      if ( value.empty() )
        {
          (*i)++;
          if ( static_cast<unsigned int>(*i) < args.size() ) 
            _extractValue( args[*i] );
          else
            throw( ArgParseException("Missing a value for this argument!",
                                     toString() ) );
        }
      else
        _extractValue( value );
                                
      _alreadySet = true;
      _checkWithVisitor();
      return true;
    }   
  else
    return false;
}

/**
 * Implementation of shortID.
 */
template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
auto ValueArg<T, T_Char, T_CharTraits, T_Alloc>::shortID(const StringType& val) const -> StringType
{
  static_cast<void>(val); // Ignore input, don't warn
  return Arg<T_Char, T_CharTraits, T_Alloc>::shortID( _typeDesc ); 
}

/**
 * Implementation of longID.
 */
template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
auto ValueArg<T, T_Char, T_CharTraits, T_Alloc>::longID(const StringType& val) const -> StringType
{
  static_cast<void>(val); // Ignore input, don't warn
  return Arg<T_Char, T_CharTraits, T_Alloc>::longID( _typeDesc ); 
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
void ValueArg<T, T_Char, T_CharTraits, T_Alloc>::_extractValue( const StringType& val )
{
  try {
    ExtractValue(_value, val, typename ArgTraits<T>::ValueCategory());
  } catch( ArgParseException &e) {
    throw ArgParseException(e.error(), toString());
  }
    
  if ( _constraint != NULL )
    if ( ! _constraint->check( _value ) )
      throw( CmdLineParseException( "Value '" + val + 
                                    + "' does not meet constraint: " 
                                    + _constraint->description(),
                                    toString() ) );
}

template<class T, typename T_Char, typename T_CharTraits, typename T_Alloc>
void ValueArg<T, T_Char, T_CharTraits, T_Alloc>::reset()
{
  Arg<T_Char, T_CharTraits, T_Alloc>::reset();
  _value = _default;
}

} // namespace TCLAP

#endif
