// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  StringConvert.h
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


#ifndef TCLAP_STRINGCONVERT_H
#define TCLAP_STRINGCONVERT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <wchar.h>
#include <cassert>
#include <cstdlib>
#include <cuchar>
#include <limits>
#include <array>
#include <memory>
#include <string_view>
#include <string>

namespace TCLAP {
	template<typename T_Char, typename T_CharTraits>
	class StringConvert;

	template<typename T_Char>
	inline consteval std::array<T_Char, std::numeric_limits<unsigned char>::max() + 1> getMappingFromBasicChar();

#define TCLAP_MAKE_GET_MAPPING_FROM_BASIC_CHAR(type_to, prefix_to) \
	template<>\
	inline consteval std::array<type_to, std::numeric_limits<unsigned char>::max() + 1> getMappingFromBasicChar<type_to>() {\
		std::array<type_to, std::numeric_limits<unsigned char>::max() + 1> arr_mapping;\
		constexpr type_to arr_basic_to[] = {\
			prefix_to##' ', prefix_to##'\t', prefix_to##'\v', prefix_to##'\f', prefix_to##'\n',\
			prefix_to##'a', prefix_to##'b', prefix_to##'c', prefix_to##'d', prefix_to##'e', prefix_to##'f', prefix_to##'g', prefix_to##'h', prefix_to##'i', prefix_to##'j', prefix_to##'k', prefix_to##'l', prefix_to##'m', prefix_to##'n', prefix_to##'o', prefix_to##'p', prefix_to##'q', prefix_to##'r', prefix_to##'s', prefix_to##'t', prefix_to##'u', prefix_to##'v', prefix_to##'w', prefix_to##'x', prefix_to##'y', prefix_to##'z',\
			prefix_to##'A', prefix_to##'B', prefix_to##'C', prefix_to##'D', prefix_to##'E', prefix_to##'F', prefix_to##'G', prefix_to##'H', prefix_to##'I', prefix_to##'J', prefix_to##'K', prefix_to##'L', prefix_to##'M', prefix_to##'N', prefix_to##'O', prefix_to##'P', prefix_to##'Q', prefix_to##'R', prefix_to##'S', prefix_to##'T', prefix_to##'U', prefix_to##'V', prefix_to##'W', prefix_to##'X', prefix_to##'Y', prefix_to##'Z',\
			prefix_to##'0', prefix_to##'1', prefix_to##'2', prefix_to##'3', prefix_to##'4', prefix_to##'5', prefix_to##'6', prefix_to##'7', prefix_to##'8', prefix_to##'9',\
			prefix_to##'_', prefix_to##'{', prefix_to##'}', prefix_to##'[', prefix_to##']', prefix_to##'#', prefix_to##'(', prefix_to##')', prefix_to##'<', prefix_to##'>', prefix_to##'%', prefix_to##':', prefix_to##';', prefix_to##'.', prefix_to##'?', prefix_to##'*', prefix_to##'+', prefix_to##'-', prefix_to##'/', prefix_to##'^', prefix_to##'&', prefix_to##'|', prefix_to##'~', prefix_to##'!', prefix_to##'=', prefix_to##',', prefix_to##'\\', prefix_to##'\"', prefix_to##'\''\
		};\
		constexpr char arr_basic_from[] = {\
			' ', '\t', '\v', '\f', '\n', \
			'a',  'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', \
			'A',  'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', \
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', \
			'_', '{', '}', '[', ']', '#', '(', ')', '<', '>', '%', ':', ';', '.', '?', '*', '+', '-', '/', '^', '&', '|', '~', '!', '=', ',', '\\', '\"', '\''\
		};\
		for (std::size_t i = 0; i < arr_mapping.size(); ++i) arr_mapping[i] = type_to{};\
		for (std::size_t i = 0; i < sizeof(arr_basic_from) / sizeof(char); ++i) arr_mapping[static_cast<unsigned char>(arr_basic_from[i])] = arr_basic_to[i];\
		return arr_mapping;\
	}

	TCLAP_MAKE_GET_MAPPING_FROM_BASIC_CHAR(char, );
	TCLAP_MAKE_GET_MAPPING_FROM_BASIC_CHAR(wchar_t, L);
	TCLAP_MAKE_GET_MAPPING_FROM_BASIC_CHAR(char8_t, u8);
	TCLAP_MAKE_GET_MAPPING_FROM_BASIC_CHAR(char16_t, u);
	TCLAP_MAKE_GET_MAPPING_FROM_BASIC_CHAR(char32_t, U);

#undef TCLAP_MAKE_GET_MAPPING_FROM_BASIC_CHAR

	template<typename T_Char>
	inline constexpr std::array<T_Char, std::numeric_limits<unsigned char>::max() + 1> mappingFromBasicChar = getMappingFromBasicChar<T_Char>();

	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>>
	class StringConvert {
	public:
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using StringViewType = std::basic_string_view<T_Char, T_CharTraits>;
		template<typename T_Alloc = std::allocator<T_Char>>
		using StringTypeTmpl = std::basic_string<T_Char, T_CharTraits, T_Alloc>;
		StringConvert(const StringConvert& rhs) = delete;
		StringConvert& operator=(const StringConvert& rhs) = delete;
		static std::string toExceptionDescription(const StringViewType& strview);
		static constexpr T_Char fromConstBasicChar(char ch_basic_from) {
			T_Char ch_basic_to{};
			if (ch_basic_from != char{}) {
				ch_basic_to = mappingFromBasicChar<T_Char>[static_cast<unsigned char>(ch_basic_from)];
				// Only basic characters are allowed.
				if (ch_basic_to == T_Char{}) { assert(false); abort(); }
			}
			return ch_basic_to;
		}
		template<std::size_t size_arr_basic_from>
		static constexpr std::array<T_Char, size_arr_basic_from - 1> fromConstBasicCharArray(const char(&arr_basic_from)[size_arr_basic_from]) {
			std::array<T_Char, size_arr_basic_from - 1> arr_str_to;
			for (std::size_t i = 0; i < size_arr_basic_from - 1; ++i) {
				T_Char ch_basic_to{};
				if (arr_basic_from[i] != char{}) {
					ch_basic_to = mappingFromBasicChar<T_Char>[static_cast<unsigned char>(arr_basic_from[i])];
					// Only basic characters are allowed.
					if (ch_basic_to == T_Char{}) { assert(false); abort(); }
				}
				arr_str_to[i] = ch_basic_to;
			}
			return arr_str_to;
		}
		template<typename T_Alloc = std::allocator<T_Char>, std::size_t size_arr_basic_from>
		static std::basic_string<T_Char, T_CharTraits, T_Alloc> fromConstBasicCharString(const char(&arr_basic_from)[size_arr_basic_from], const T_Alloc& alloc = T_Alloc()) {
			std::array<T_Char, size_arr_basic_from - 1> arr_str_to = fromConstBasicCharArray(arr_basic_from);
			return std::basic_string<T_Char, T_CharTraits, T_Alloc>(arr_str_to.data(), arr_str_to.size(), alloc);
		}
	};

	template<>
	inline std::string StringConvert<char, std::char_traits<char>>::toExceptionDescription(const StringViewType& strview) {
		return std::string(strview);
	}

	template<>
	inline std::string StringConvert<wchar_t, std::char_traits<wchar_t>>::toExceptionDescription(const StringViewType& strview) {
		std::string str;
		std::mbstate_t mbstate{};
		char mbch[MB_LEN_MAX]{};
		std::size_t result_mbconv = 0;
		for (const CharType& ch : strview) {
			errno_t errno_mbconv = wcrtomb_s(&result_mbconv, mbch, ch, &mbstate);
			if (errno_mbconv || result_mbconv == static_cast<std::size_t>(-1)) {
				mbstate = mbstate_t{};
				result_mbconv = std::c16rtomb(mbch, u'\ufffd', &mbstate);
				mbstate = mbstate_t{};
			}
			assert(result_mbconv <= MB_LEN_MAX);
			str.append(mbch, result_mbconv);
		}
		{
			errno_t errno_mbconv = wcrtomb_s(&result_mbconv, mbch, CharType{}, &mbstate);
			if (errno_mbconv || result_mbconv == static_cast<std::size_t>(-1)) {
				mbstate = mbstate_t{};
				result_mbconv = std::c16rtomb(mbch, u'\ufffd', &mbstate);
				mbstate = mbstate_t{};
			}
			assert(result_mbconv <= MB_LEN_MAX);
			assert(result_mbconv > 0);
			str.append(mbch, result_mbconv - 1);
		}
		return str;
	}

#if \
defined(__cpp_char8_t) \
&& __cpp_char8_t >= 201811L\
&& defined(__cpp_lib_char8_t) \
&& __cpp_lib_char8_t >= 201907L \
&& false // TODO: Enable this block of code after there's support for std::c8rtomb in major compilers.
	template<>
	inline std::string StringConvert<char8_t, std::char_traits<char8_t>>::toExceptionDescription(const StringViewType& strview) {
		std::string str;
		std::mbstate_t mbstate{};
		char mbch[MB_LEN_MAX]{};
		std::size_t result_mbconv = 0;
		for (const CharType& ch : strview) {
			result_mbconv = std::c8rtomb(mbch, ch, &mbstate);
			if (result_mbconv == static_cast<std::size_t>(-1)) {
				mbstate = mbstate_t{};
				result_mbconv = std::c16rtomb(mbch, u'\ufffd', &mbstate);
				mbstate = mbstate_t{};
			}
			assert(result_mbconv <= MB_LEN_MAX);
			str.append(mbch, result_mbconv);
		}
		{
			result_mbconv = std::c8rtomb(mbch, CharType{}, &mbstate);
			if (result_mbconv == static_cast<std::size_t>(-1)) {
				mbstate = mbstate_t{};
				result_mbconv = std::c16rtomb(mbch, u'\ufffd', &mbstate);
				mbstate = mbstate_t{};
			}
			assert(result_mbconv <= MB_LEN_MAX);
			assert(result_mbconv > 0);
			str.append(mbch, result_mbconv - 1);
		}
		return str;
	}
#endif

	template<>
	inline std::string StringConvert<char16_t, std::char_traits<char16_t>>::toExceptionDescription(const StringViewType& strview) {
		std::string str;
		std::mbstate_t mbstate{};
		char mbch[MB_LEN_MAX]{};
		std::size_t result_mbconv = 0;
		for (const CharType& ch : strview) {
			result_mbconv = std::c16rtomb(mbch, ch, &mbstate);
			if (result_mbconv == static_cast<std::size_t>(-1)) {
				mbstate = mbstate_t{};
				result_mbconv = std::c16rtomb(mbch, u'\ufffd', &mbstate);
				mbstate = mbstate_t{};
			}
			assert(result_mbconv <= MB_LEN_MAX);
			str.append(mbch, result_mbconv);
		}
		{
			result_mbconv = std::c16rtomb(mbch, CharType{}, &mbstate);
			if (result_mbconv == static_cast<std::size_t>(-1)) {
				mbstate = mbstate_t{};
				result_mbconv = std::c16rtomb(mbch, u'\ufffd', &mbstate);
				mbstate = mbstate_t{};
			}
			assert(result_mbconv <= MB_LEN_MAX);
			assert(result_mbconv > 0);
			str.append(mbch, result_mbconv - 1);
		}
		return str;
	}

	template<>
	inline std::string StringConvert<char32_t, std::char_traits<char32_t>>::toExceptionDescription(const StringViewType& strview) {
		std::string str;
		std::mbstate_t mbstate{};
		char mbch[MB_LEN_MAX]{};
		std::size_t result_mbconv = 0;
		for (const CharType& ch : strview) {
			result_mbconv = std::c32rtomb(mbch, ch, &mbstate);
			if (result_mbconv == static_cast<std::size_t>(-1)) {
				mbstate = mbstate_t{};
				result_mbconv = std::c16rtomb(mbch, u'\ufffd', &mbstate);
				mbstate = mbstate_t{};
			}
			assert(result_mbconv <= MB_LEN_MAX);
			str.append(mbch, result_mbconv);
		}
		{
			result_mbconv = std::c32rtomb(mbch, CharType{}, &mbstate);
			if (result_mbconv == static_cast<std::size_t>(-1)) {
				mbstate = mbstate_t{};
				result_mbconv = std::c16rtomb(mbch, u'\ufffd', &mbstate);
				mbstate = mbstate_t{};
			}
			assert(result_mbconv <= MB_LEN_MAX);
			assert(result_mbconv > 0);
			str.append(mbch, result_mbconv - 1);
		}
		return str;
	}
} //namespace TCLAP

#endif
