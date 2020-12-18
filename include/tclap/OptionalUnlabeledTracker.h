// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-



/****************************************************************************** 
 * 
 *  file:  OptionalUnlabeledTracker.h
 * 
 *  Copyright (c) 2005, Michael E. Smoot .
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


#ifndef TCLAP_OPTIONAL_UNLABELED_TRACKER_H
#define TCLAP_OPTIONAL_UNLABELED_TRACKER_H

#include <string>

namespace TCLAP {
	
template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>, typename T_Alloc = std::allocator<T_Char>>
class OptionalUnlabeledTracker
{
	public:
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using AllocatorType = T_Alloc;
		using StringType = std::basic_string<T_Char, T_CharTraits, T_Alloc>;
		using OptionalUnlabeledTrackerType = OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>;
		static void check( bool req, const StringType& argName );
		static void gotOptional() { alreadyOptionalRef() = true; }
		static bool& alreadyOptional() { return alreadyOptionalRef(); } 
	private:
		static bool& alreadyOptionalRef() { static bool ct = false; return ct; }
};


template<typename T_Char, typename T_CharTraits, typename T_Alloc>
inline void OptionalUnlabeledTracker<T_Char, T_CharTraits, T_Alloc>::check( bool req, const StringType& argName )
{
    if ( OptionalUnlabeledTracker::alreadyOptional() )
        throw( SpecificationException<T_Char, T_CharTraits, T_Alloc>( StringConvertType::fromConstBasicCharString("You can't specify ANY Unlabeled Arg following an optional Unlabeled Arg"), argName ) );

    if ( !req )
        OptionalUnlabeledTracker::gotOptional();
}


} // namespace TCLAP

#endif
