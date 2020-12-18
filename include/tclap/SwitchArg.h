// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/****************************************************************************** 
 * 
 *  file:  SwitchArg.h
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


#ifndef TCLAP_SWITCH_ARG_H
#define TCLAP_SWITCH_ARG_H

#include <string>
#include <vector>

#include <tclap/Arg.h>

namespace TCLAP {

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
class SwitchArg;

/**
 * A simple switch argument.  If the switch is set on the command line, then
 * the getValue method will return the opposite of the default value for the
 * switch.
 */
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class SwitchArg : public Arg<T_Char, T_CharTraits, T_Alloc>
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

  /**
   * SwitchArg constructor.
   * \param flag - The one character flag that identifies this
   * argument on the command line.
   * \param name - A one word name for the argument.  Can be
   * used as a long flag on the command line.
   * \param desc - A description of what the argument is for or
   * does.
   * \param def - The default value for this Switch. 
   * \param v - An optional visitor.  You probably should not
   * use this unless you have a very good reason.
   */
  SwitchArg(const StringType& flag, 
	    const StringType& name, 
	    const StringType& desc,
	    bool def = false,
	    Visitor* v = nullptr,
        const AllocatorType& alloc = AllocatorType());

                                  
  /**
   * SwitchArg constructor.
   * \param flag - The one character flag that identifies this
   * argument on the command line.
   * \param name - A one word name for the argument.  Can be
   * used as a long flag on the command line.
   * \param desc - A description of what the argument is for or
   * does.
   * \param parser - A CmdLine parser object to add this Arg to
   * \param def - The default value for this Switch.
   * \param v - An optional visitor.  You probably should not
   * use this unless you have a very good reason.
   */
  SwitchArg(const StringType& flag, 
	    const StringType& name, 
	    const StringType& desc,
	    CmdLineInterfaceType& parser,
	    bool def = false,
	    Visitor* v = nullptr,
        const AllocatorType& alloc = AllocatorType());
                                  
                                  
  /**
   * Handles the processing of the argument.
   * This re-implements the Arg version of this method to set the
   * _value of the argument appropriately.
   * \param i - Pointer the the current argument in the list.
   * \param args - Mutable list of strings. Passed
   * in from main().
   */
  virtual bool processArg(int* i, StringVectorType& args); 

  /**
   * Checks a string to see if any of the chars in the string
   * match the flag for this Switch.
   */
  bool combinedSwitchesMatch(StringType& combined);

  /**
   * Returns bool, whether or not the switch has been set.
   */
  bool getValue() const { return _value; }

  /**
   * A SwitchArg can be used as a boolean, indicating
   * whether or not the switch has been set. This is the
   * same as calling getValue()
   */
  operator bool() const { return _value; }
                
  virtual void reset();

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
  bool _value;

  /**
   * Used to support the reset() method so that ValueArg can be
   * reset to their constructed value.
   */
  bool _default;

private:
  /**
   * Checks to see if we've found the last match in
   * a combined string.
   */
  bool lastCombined(StringType& combined);

  /**
   * Does the common processing of processArg.
   */
  void commonProcessing();
};

//////////////////////////////////////////////////////////////////////
//BEGIN SwitchArg.cpp
//////////////////////////////////////////////////////////////////////
template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline SwitchArg<T_Char, T_CharTraits, T_Alloc>::SwitchArg(const StringType& flag,
			    const StringType& name, 
			    const StringType& desc, 
			    bool default_val,
			    Visitor* v,
                const AllocatorType& alloc )
  : Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, false, false, v, alloc),
    _value( default_val ),
    _default( default_val )
{ }

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline SwitchArg<T_Char, T_CharTraits, T_Alloc>::SwitchArg(const StringType& flag,
			    const StringType& name, 
			    const StringType& desc, 
			    CmdLineInterfaceType& parser,
			    bool default_val,
			    Visitor* v,
                const AllocatorType& alloc )
  : Arg<T_Char, T_CharTraits, T_Alloc>(flag, name, desc, false, false, v, alloc),
    _value( default_val ),
    _default(default_val)
{ 
  parser.add( this );
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline bool SwitchArg<T_Char, T_CharTraits, T_Alloc>::lastCombined(StringType& combinedSwitches )
{
  for ( unsigned int i = 1; i < combinedSwitches.length(); i++ )
    if ( combinedSwitches[i] != Arg<T_Char, T_CharTraits, T_Alloc>::blankChar() )
      return false;
        
  return true;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline bool SwitchArg<T_Char, T_CharTraits, T_Alloc>::combinedSwitchesMatch(StringType& combinedSwitches )
{
  // make sure this is actually a combined switch
  if ( combinedSwitches.length() > 0 &&
       combinedSwitches[0] != Arg<T_Char, T_CharTraits, T_Alloc>::flagStartString()[0] )
    return false;

  // make sure it isn't a long name 
  if ( combinedSwitches.substr( 0, Arg<T_Char, T_CharTraits, T_Alloc>::nameStartString().length() ) ==
       Arg<T_Char, T_CharTraits, T_Alloc>::nameStartString() )
    return false;

  // make sure the delimiter isn't in the string 
  if ( combinedSwitches.find_first_of(Arg<T_Char, T_CharTraits, T_Alloc>::delimiter()) != StringType::npos)
    return false;

  // ok, we're not specifying a ValueArg, so we know that we have
  // a combined switch list.  
  for ( unsigned int i = 1; i < combinedSwitches.length(); i++ )
    if ( _flag.length() > 0 && 
	 combinedSwitches[i] == _flag[0] &&
	 _flag[0] != Arg<T_Char, T_CharTraits, T_Alloc>::flagStartString()[0] )
      {
	// update the combined switches so this one is no longer present
	// this is necessary so that no unlabeled args are matched
	// later in the processing.
	//combinedSwitches.erase(i,1);
	combinedSwitches[i] = Arg<T_Char, T_CharTraits, T_Alloc>::blankChar();
	return true;
      }

  // none of the switches passed in the list match. 
  return false;   
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void SwitchArg<T_Char, T_CharTraits, T_Alloc>::commonProcessing()
{
  if ( _xorSet )
    throw(CmdLineParseException(
				"Mutually exclusive argument already set!", toString()));

  if ( _alreadySet ) 
    throw(CmdLineParseException("Argument already set!", toString()));

  _alreadySet = true;

  if ( _value == true )
    _value = false;
  else
    _value = true;

  _checkWithVisitor();
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline bool SwitchArg<T_Char, T_CharTraits, T_Alloc>::processArg(int *i, StringVectorType& args)
{
  if ( _ignoreable && Arg<T_Char, T_CharTraits, T_Alloc>::ignoreRest() )
    return false;

  // if the whole string matches the flag or name string
  if ( argMatches( args[*i] ) )
    {
      commonProcessing();

      return true;
    }
  // if a substring matches the flag as part of a combination
  else if ( combinedSwitchesMatch( args[*i] ) )
    {
      // check again to ensure we don't misinterpret 
      // this as a MultiSwitchArg 
      if ( combinedSwitchesMatch( args[*i] ) )
	throw(CmdLineParseException("Argument already set!", 
				    toString()));

      commonProcessing();

      // We only want to return true if we've found the last combined
      // match in the string, otherwise we return true so that other 
      // switches in the combination will have a chance to match.
      return lastCombined( args[*i] );
    }
  else
    return false;
}

template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void SwitchArg<T_Char, T_CharTraits, T_Alloc>::reset()
{
  Arg<T_Char, T_CharTraits, T_Alloc>::reset();
  _value = _default;  
}
//////////////////////////////////////////////////////////////////////
//End SwitchArg.cpp
//////////////////////////////////////////////////////////////////////

} //namespace TCLAP

#endif
