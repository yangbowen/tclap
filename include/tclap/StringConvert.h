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
#include <algorithm>
#include <vector>
#include <string_view>
#include <string>
#include <streambuf>
#include <iostream>

namespace TCLAP {
	template<typename T_Char, typename T_CharTraits>
	class StringConvert;

	template<typename T_Char>
	struct CharMapping {};

	template<typename T_Char>
	inline consteval std::array<T_Char, std::numeric_limits<unsigned char>::max() + 1> getMappingFromBasicChar();

#define TCLAP_MAKE_CHAR_MAPPINGS(type_to, prefix_to) \
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
	}\
\
	template<>\
	struct CharMapping<type_to> {\
		using CharType = type_to;\
		static constexpr CharType arrInvalidChar[] = prefix_to##"\ufffd";\
		static_assert(sizeof(arrInvalidChar) / sizeof(CharType) >= 1, "The invalid character array is too short.");\
	};

	TCLAP_MAKE_CHAR_MAPPINGS(char, );
	TCLAP_MAKE_CHAR_MAPPINGS(wchar_t, L);
#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L
	TCLAP_MAKE_CHAR_MAPPINGS(char8_t, u8);
#endif
	TCLAP_MAKE_CHAR_MAPPINGS(char16_t, u);
	TCLAP_MAKE_CHAR_MAPPINGS(char32_t, U);

#undef TCLAP_MAKE_CHAR_MAPPINGS

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
		using StreambufType = std::basic_streambuf<T_Char, T_CharTraits>;
		using IstreamType = std::basic_istream<T_Char, T_CharTraits>;
		using OstreamType = std::basic_ostream<T_Char, T_CharTraits>;
		StringConvert(const StringConvert& rhs) = delete;
		StringConvert& operator=(const StringConvert& rhs) = delete;
		static constexpr CharType fromConstBasicChar(char ch_basic_from) {
			CharType ch_basic_to{};
			if (!std::char_traits<char>::eq(ch_basic_from, char{})) {
				ch_basic_to = mappingFromBasicChar<CharType>[static_cast<unsigned char>(ch_basic_from)];
				// Only basic characters are allowed.
				if (CharTraitsType::eq(ch_basic_to, CharType{})) { assert(false); abort(); }
			}
			return ch_basic_to;
		}
		template<std::size_t size_arr_basic_from>
		static constexpr std::array<CharType, size_arr_basic_from - 1> fromConstBasicCharArray(const char(&arr_basic_from)[size_arr_basic_from]) {
			std::array<CharType, size_arr_basic_from - 1> arr_str_to;
			for (std::size_t i = 0; i < size_arr_basic_from - 1; ++i) {
				CharType ch_basic_to{};
				if (!std::char_traits<char>::eq(arr_basic_from[i], char{})) {
					ch_basic_to = mappingFromBasicChar<CharType>[static_cast<unsigned char>(arr_basic_from[i])];
					// Only basic characters are allowed.
					if (CharTraitsType::eq(ch_basic_to, CharType{})) { assert(false); abort(); }
				}
				arr_str_to[i] = ch_basic_to;
			}
			return arr_str_to;
		}
		template<typename T_Alloc = std::allocator<T_Char>, std::size_t size_arr_basic_from>
		static StringTypeTmpl<T_Alloc> fromConstBasicCharString(const char(&arr_basic_from)[size_arr_basic_from], const T_Alloc& alloc = T_Alloc()) {
			std::array<CharType, size_arr_basic_from - 1> arr_str_to = fromConstBasicCharArray(arr_basic_from);
			return StringTypeTmpl<T_Alloc>(arr_str_to.data(), arr_str_to.size(), alloc);
		}
		static std::size_t toMBCharRestartable(char(&mbch)[MB_LEN_MAX], const CharType& ch, std::mbstate_t& mbstate);
		static std::size_t fromMBCharRestartable(CharType& ch, const char* p_mbs, std::size_t size_mbs, std::mbstate_t& mbstate);
		template<typename T_Alloc = std::allocator<char>>
		static std::basic_string<char, std::char_traits<char>, T_Alloc> toMBStringRestartable(const StringViewType& strview, std::mbstate_t& mbstate) {
			std::basic_string<char, std::char_traits<char>, T_Alloc> str;
			char mbch[MB_LEN_MAX]{};
			std::size_t result_mbconv = 0;
			for (const CharType& ch : strview) {
				result_mbconv = toMBCharRestartable(mbch, ch, &mbstate);
				if (result_mbconv == static_cast<std::size_t>(-1)) {
					mbstate = mbstate_t{};
					str.append(
						CharMapping<char>::arrInvalidChar,
						CharMapping<char>::arrInvalidChar + (sizeof(CharMapping<char>::arrInvalidChar) / sizeof(CharMapping<char>::CharType) - 1)
					);
				} else {
					assert(result_mbconv <= MB_LEN_MAX);
					str.append(mbch, result_mbconv);
				}
			}
			return str;
		}
		template<typename T_Alloc = std::allocator<T_Char>>
		static StringTypeTmpl<T_Alloc> fromMBStringRestartable(const std::basic_string_view<char, std::char_traits<char>>& strview, std::mbstate_t& mbstate) {
			std::basic_string_view<char, std::char_traits<char>> strview_temp = strview;
			StringTypeTmpl<T_Alloc> str;
			CharType ch{};
			std::size_t result_mbconv = 0;
			for (; !strview_temp.empty(); strview_temp.remove_prefix(result_mbconv)) {
				result_mbconv = std::mbrtoc16(&ch, strview_temp.data(), strview_temp.size(), &mbstate);
				if (result_mbconv <= strview_temp.size()) {
					if (!result_mbconv) result_mbconv = 1;
					// Written one, read some.
					str.push_back(ch);
				} else {
					switch (result_mbconv) {
						case -1:
							mbstate = mbstate_t{};
							str.append(
								CharMapping<T_Char>::arrInvalidChar,
								CharMapping<T_Char>::arrInvalidChar + (sizeof(CharMapping<T_Char>::arrInvalidChar) / sizeof(CharMapping<T_Char>::CharType) - 1)
							);
							result_mbconv = 1;
							break;
						case -2:
							// Written none, read all.
							result_mbconv = strview_temp.size();
							break;
						case -3:
							// Written one, read none.
							str.push_back(ch);
							result_mbconv = 0;
							break;
						default:
							assert(false);abort();
					}
				}
				assert(result_mbconv >= 0 && result_mbconv <= strview_temp.size());
			}
			return str;
		}
		template<typename T_Alloc = std::allocator<char>>
		static std::basic_string<char, std::char_traits<char>, T_Alloc> toMBString(const StringViewType& strview) {
			std::mbstate_t mbstate{};
			std::basic_string<char, std::char_traits<char>, T_Alloc> str = toMBStringRestartable(strview, mbstate);
			{
				char mbch[MB_LEN_MAX]{};
				std::size_t result_mbconv = toMBCharRestartable(mbch, CharType{}, mbstate);
				if (result_mbconv == static_cast<std::size_t>(-1)) {
					mbstate = mbstate_t{};
					str.append(
						CharMapping<char>::arrInvalidChar,
						CharMapping<char>::arrInvalidChar + (sizeof(CharMapping<char>::arrInvalidChar) / sizeof(CharMapping<char>::CharType) - 1)
					);
				} else {
					assert(result_mbconv <= MB_LEN_MAX);
					assert(result_mbconv > 0);
					assert(CharTraitsType::eq(mbch[result_mbconv - 1], char{}));
					str.append(mbch, result_mbconv - 1);
				}
			}
			return str;
		}
		template<typename T_Alloc = std::allocator<T_Char>>
		static StringTypeTmpl<T_Alloc> fromMBString(const std::basic_string_view<char, std::char_traits<char>>& strview) {
			std::mbstate_t mbstate{};
			StringTypeTmpl<T_Alloc> str = fromMBStringRestartable(strview, mbstate);
			{
				CharType ch{};
				const char mbch_null{};
				while (true) {
					std::size_t result_mbconv = fromMBCharRestartable(ch, &mbch_null, 1, mbstate);
					bool is_pending_append_ch = false;
					if (result_mbconv <= 1) {
						is_pending_append_ch = true;
					} else {
						switch (result_mbconv) {
							case -1:
								mbstate = mbstate_t{};
								str.append(
									CharMapping<T_Char>::arrInvalidChar,
									CharMapping<T_Char>::arrInvalidChar + (sizeof(CharMapping<T_Char>::arrInvalidChar) / sizeof(CharMapping<T_Char>::CharType) - 1)
								);
								ch = CharType{};
								is_pending_append_ch = true;
								break;
							case -2:
								// Written none, read all.
								is_pending_append_ch = false;
								break;
							case -3:
								// Written one, read none.
								is_pending_append_ch = true;
								break;
							default:
								assert(false);abort();
						}
					}
					if (is_pending_append_ch) {
						if (CharTraitsType::eq(ch, CharType{})) {
							break;
						}
						str.push_back(ch);
					}
				}
			}
			return str;
		}
		class ConvertedIstreamBuf final : public StreambufType {
		public:
			using char_type = T_Char;
			using traits_type = T_CharTraits;
			using int_type = typename T_CharTraits::int_type;
			using pos_type = typename T_CharTraits::pos_type;
			using off_type = typename T_CharTraits::off_type;
			explicit ConvertedIstreamBuf(StreambufType* streambuf_wrapped)
				: StreambufType(),
				_streambuf_wrapped(streambuf_wrapped),
				_vecbuf_buffer(_size_buffer, char_type{}) {
				assert(_streambuf_wrapped);
				StreambufType::setg(nullptr, nullptr, nullptr);
			}
			ConvertedIstreamBuf(const ConvertedIstreamBuf& rhs)
				: StreambufType(static_cast<const StreambufType&>(rhs)),
				_streambuf_wrapped(rhs._streambuf_wrapped),
				_mbstate(rhs._mbstate),
				_vecbuf_buffer(rhs._vecbuf_buffer) {
				StreambufType::setg(
					StreambufType::eback() ? _vecbuf_buffer.data() : nullptr,
					StreambufType::gptr() ? _vecbuf_buffer.data() + (StreambufType::gptr() - StreambufType::eback()) : nullptr,
					StreambufType::egptr() ? _vecbuf_buffer.data() + (StreambufType::egptr() - StreambufType::eback()) : nullptr
				);
			}
			ConvertedIstreamBuf(ConvertedIstreamBuf&& rhs)
				: StreambufType(std::move(static_cast<StreambufType&>(rhs))),
				_streambuf_wrapped(std::move(rhs._streambuf_wrapped)),
				_mbstate(std::move(rhs._mbstate)),
				_vecbuf_buffer(std::move(rhs._vecbuf_buffer)) {
				StreambufType::setg(
					StreambufType::eback() ? _vecbuf_buffer.data() : nullptr,
					StreambufType::gptr() ? _vecbuf_buffer.data() + (StreambufType::gptr() - StreambufType::eback()) : nullptr,
					StreambufType::egptr() ? _vecbuf_buffer.data() + (StreambufType::egptr() - StreambufType::eback()) : nullptr
				);
			}
			virtual ~ConvertedIstreamBuf() override {
				if (_streambuf_wrapped) StreambufType::pubsync();
			}
			ConvertedIstreamBuf& operator=(const ConvertedIstreamBuf& rhs) {
				static_cast<StreambufType&>(*this) = static_cast<const StreambufType&>(rhs);
				_streambuf_wrapped = rhs._streambuf_wrapped;
				_mbstate = rhs._mbstate;
				_vecbuf_buffer = rhs._vecbuf_buffer;
				StreambufType::setg(
					StreambufType::eback() ? _vecbuf_buffer.data() : nullptr,
					StreambufType::gptr() ? _vecbuf_buffer.data() + (StreambufType::gptr() - StreambufType::eback()) : nullptr,
					StreambufType::egptr() ? _vecbuf_buffer.data() + (StreambufType::egptr() - StreambufType::eback()) : nullptr
				);
				return *this;
			}
			ConvertedIstreamBuf& operator=(ConvertedIstreamBuf&& rhs) {
				static_cast<StreambufType&>(*this) = std::move(static_cast<StreambufType&>(rhs));
				_streambuf_wrapped = std::move(rhs._streambuf_wrapped);
				_mbstate = std::move(rhs._mbstate);
				_vecbuf_buffer = std::move(rhs._vecbuf_buffer);
				StreambufType::setg(
					StreambufType::eback() ? _vecbuf_buffer.data() : nullptr,
					StreambufType::gptr() ? _vecbuf_buffer.data() + (StreambufType::gptr() - StreambufType::eback()) : nullptr,
					StreambufType::egptr() ? _vecbuf_buffer.data() + (StreambufType::egptr() - StreambufType::eback()) : nullptr
				);
				return *this;
			}
		protected:
			StreambufType* _streambuf_wrapped = nullptr;
			mbstate_t _mbstate{};
			std::vector<char_type> _vecbuf_buffer;
			virtual int_type underflow() override {
				assert(_streambuf_wrapped);
				{
					std::vector<char> vecbuf_unconv;
					std::streamsize count_mbchar_get = std::clamp<std::streamsize>(_streambuf_wrapped->showmanyc(), 1, _count_mbchar_get_max);
					assert(count_mbchar_get > 0);
					vecbuf_unconv.resize(count_mbchar_get);
					count_mbchar_get = std::max<std::streamsize>(_streambuf_wrapped->sgetn(vecbuf_unconv.data(), count_mbchar_get), 0);
					assert(count_mbchar_get > 0);
					vecbuf_unconv.resize(count_mbchar_get);
					if (!vecbuf_unconv.empty()) {
						if (StreambufType::gptr() != StreambufType::eback()) {
							assert(StreambufType::eback());
							assert(StreambufType::gptr());
							_vecbuf_buffer.erase(_vecbuf_buffer.cbegin(), _vecbuf_buffer.cbegin() + (StreambufType::egptr() - StreambufType::eback()));
						}
						{
							StringTypeTmpl<std::allocator<char_type>> str = fromMBStringRestartable<std::allocator<char_type>>(std::basic_string_view<char, std::char_traits<char>>(vecbuf_unconv.data(), vecbuf_unconv.size()), _mbstate);
							_vecbuf_buffer.insert(_vecbuf_buffer.cend(), str.begin(), str.end());
						}
					}
				}
				if (!_vecbuf_buffer.empty()) {
					StreambufType::setg(_vecbuf_buffer.data(), _vecbuf_buffer.data() + (StreambufType::gptr() - StreambufType::eback()), _vecbuf_buffer.data() + _vecbuf_buffer.size());
					return traits_type::to_int_type(*StreambufType::gptr());
				} else {
					StreambufType::setg(nullptr, nullptr, nullptr);
					return traits_type::eof();
				}
			}
			virtual int_type uflow() override {
				int_type int_ch = underflow();
				if (!traits_type::eq_int_type(int_ch, traits_type::eof())) {
					assert(StreambufType::gptr() && StreambufType::gptr() != StreambufType::egptr());
					StreambufType::gbump(1);
				}
				return int_ch;
			}
			virtual std::streamsize xsgetn(char_type* ptr, std::streamsize size_get) override {
				char_type* ptr_temp = ptr;
				std::streamsize size_get_remaining = size_get;
				while (size_get_remaining > 0) {
					if (traits_type::eq_int_type(underflow(), traits_type::eof())) break;
					assert(StreambufType::gptr() && StreambufType::gptr() != StreambufType::egptr());
					std::streamsize size_copy = std::min(size_get_remaining, StreambufType::egptr() - StreambufType::gptr());
					assert(size_copy > 0);
					assert(ptr);
					std::copy(StreambufType::gptr(), StreambufType::gptr() + size_copy, ptr_temp);
					StreambufType::setg(StreambufType::eback(), StreambufType::gptr() + size_copy, StreambufType::egptr());
					ptr_temp += size_copy;
					size_get_remaining -= size_copy;
				}
				assert(size_get_remaining >= 0);
				return size_get - size_get_remaining;
			}
			virtual int sync() override {
				assert(_streambuf_wrapped);
				_streambuf_wrapped->pubsync();
				return 0;
			}
		private:
			static constexpr std::size_t _size_buffer = 0x100;
			static constexpr std::streamsize _count_mbchar_get_max = 0x100;
		};
		class ConvertedOstreamBuf final : public StreambufType {
		public:
			using char_type = T_Char;
			using traits_type = T_CharTraits;
			using int_type = typename T_CharTraits::int_type;
			using pos_type = typename T_CharTraits::pos_type;
			using off_type = typename T_CharTraits::off_type;
			explicit ConvertedOstreamBuf(StreambufType* streambuf_wrapped)
				: StreambufType(),
				_streambuf_wrapped(streambuf_wrapped),
				_vecbuf_buffer(_size_buffer, char_type{}) {
				assert(_streambuf_wrapped);
				StreambufType::setp(_vecbuf_buffer.data(), _vecbuf_buffer.data(), _vecbuf_buffer.data() + _vecbuf_buffer.size());
			}
			ConvertedOstreamBuf(const ConvertedOstreamBuf& rhs)
				: StreambufType(static_cast<const StreambufType&>(rhs)),
				_streambuf_wrapped(rhs._streambuf_wrapped),
				_mbstate(rhs._mbstate),
				_vecbuf_buffer(rhs._vecbuf_buffer) {
				StreambufType::setp(
					StreambufType::pbase() ? _vecbuf_buffer.data() : nullptr,
					StreambufType::pptr() ? _vecbuf_buffer.data() + (StreambufType::pptr() - StreambufType::pbase()) : nullptr,
					StreambufType::epptr() ? _vecbuf_buffer.data() + (StreambufType::epptr() - StreambufType::pbase()) : nullptr
				);
			}
			ConvertedOstreamBuf(ConvertedOstreamBuf&& rhs)
				: StreambufType(std::move(static_cast<StreambufType&>(rhs))),
				_streambuf_wrapped(std::move(rhs._streambuf_wrapped)),
				_mbstate(std::move(rhs._mbstate)),
				_vecbuf_buffer(std::move(rhs._vecbuf_buffer)) {
				StreambufType::setp(
					StreambufType::pbase() ? _vecbuf_buffer.data() : nullptr,
					StreambufType::pptr() ? _vecbuf_buffer.data() + (StreambufType::pptr() - StreambufType::pbase()) : nullptr,
					StreambufType::epptr() ? _vecbuf_buffer.data() + (StreambufType::epptr() - StreambufType::pbase()) : nullptr
				);
			}
			virtual ~ConvertedOstreamBuf() override {
				if (_streambuf_wrapped) StreambufType::pubsync();
			}
			ConvertedOstreamBuf& operator=(const ConvertedOstreamBuf& rhs) {
				static_cast<StreambufType&>(*this) = static_cast<const StreambufType&>(rhs);
				_streambuf_wrapped = rhs._streambuf_wrapped;
				_mbstate = rhs._mbstate;
				_vecbuf_buffer = rhs._vecbuf_buffer;
				StreambufType::setp(
					StreambufType::pbase() ? _vecbuf_buffer.data() : nullptr,
					StreambufType::pptr() ? _vecbuf_buffer.data() + (StreambufType::pptr() - StreambufType::pbase()) : nullptr,
					StreambufType::epptr() ? _vecbuf_buffer.data() + (StreambufType::epptr() - StreambufType::pbase()) : nullptr
				);
				return *this;
			}
			ConvertedOstreamBuf& operator=(ConvertedOstreamBuf&& rhs) {
				static_cast<StreambufType&>(*this) = std::move(static_cast<StreambufType&>(rhs));
				_streambuf_wrapped = std::move(rhs._streambuf_wrapped);
				_mbstate = std::move(rhs._mbstate);
				_vecbuf_buffer = std::move(rhs._vecbuf_buffer);
				StreambufType::setp(
					StreambufType::pbase() ? _vecbuf_buffer.data() : nullptr,
					StreambufType::pptr() ? _vecbuf_buffer.data() + (StreambufType::pptr() - StreambufType::pbase()) : nullptr,
					StreambufType::epptr() ? _vecbuf_buffer.data() + (StreambufType::epptr() - StreambufType::pbase()) : nullptr
				);
				return *this;
			}
		protected:
			StreambufType* _streambuf_wrapped = nullptr;
			mbstate_t _mbstate{};
			std::vector<char_type> _vecbuf_buffer;
			virtual int_type overflow(int_type int_ch = traits_type::eof()) override {
				assert(_streambuf_wrapped);
				std::string mbstr;
				if (StreambufType::pptr() != StreambufType::pbase()) {
					mbstr = toMBStringRestartable<std::allocator<char>>(StringViewType(StreambufType::pbase(), StreambufType::pptr()), _mbstate);
				}
				if (!traits_type::eq_int_type(int_ch, traits_type::eof())) {
					char_type ch = traits_type::to_char_type(int_ch);
					mbstr += toMBStringRestartable<std::allocator<char>>(StringViewType(&ch, 1), _mbstate);
				}
				assert(mbstr.size() <= std::numeric_limits<std::streamsize>::max());
				std::streamsize size_put = _streambuf_wrapped->sputn(mbstr.data(), static_cast<std::streamsize>(mbstr.size()));
				if (size_put == mbstr.size()) {
					StreambufType::setp(StreambufType::pbase(), StreambufType::epptr());
					return traits_type::not_eof(traits_type::eof());
				} else {
					StreambufType::setp(nullptr, nullptr);
					return traits_type::eof();
				}
			}
			virtual std::streamsize xsputn(const char_type* ptr, std::streamsize size_put) override {
				const char_type* ptr_temp = ptr;
				std::streamsize size_put_remaining = size_put;
				while (size_put_remaining > 0) {
					std::streamsize size_copy = std::min(size_put_remaining, StreambufType::epptr() - StreambufType::pptr());
					assert(size_copy >= 0);
					if (size_copy > 0) {
						assert(ptr);
						std::copy(ptr_temp, ptr_temp + size_copy, StreambufType::pptr());
						StreambufType::setp(StreambufType::pbase(), StreambufType::pptr() + size_copy, StreambufType::epptr());
						ptr_temp += size_copy;
						size_put_remaining -= size_copy;
					}
					if (traits_type::eq_int_type(overflow(), traits_type::eof())) break;
					assert(StreambufType::pptr() && StreambufType::pptr() != StreambufType::epptr());
				}
				assert(size_put_remaining >= 0);
				return size_put - size_put_remaining;
			}
			virtual int sync() override {
				assert(_streambuf_wrapped);
				overflow();
				_streambuf_wrapped->pubsync();
				return 0;
			}
		private:
			static constexpr std::size_t _size_buffer = 0x100;
		};
		class ConvertedIstream final : public IstreamType {
		public:
			using char_type = T_Char;
			using traits_type = T_CharTraits;
			using int_type = typename T_CharTraits::int_type;
			using pos_type = typename T_CharTraits::pos_type;
			using off_type = typename T_CharTraits::off_type;
			explicit ConvertedIstream(StreambufType* streambuf_wrapped)
				: IstreamType(nullptr),
				_streambuf(streambuf_wrapped) {
				basic_ios<T_Char, T_CharTraits>::set_rdbuf(&_streambuf);
			}
			ConvertedIstream(const ConvertedIstream&) = delete;
			ConvertedIstream(ConvertedIstream&& rhs)
				: IstreamType(std::move(static_cast<IstreamType&>(rhs))),
				_streambuf(std::move(rhs._streambuf)) {
				basic_ios<T_Char, T_CharTraits>::set_rdbuf(&_streambuf);
				rhs.basic_ios<T_Char, T_CharTraits>::set_rdbuf(&rhs._streambuf);
			}
			virtual ~ConvertedIstream() override = default;
			ConvertedIstream& operator=(const ConvertedIstream&) = delete;
			ConvertedIstream& operator=(ConvertedIstream&& rhs) {
				static_cast<IstreamType&>(*this) = std::move(static_cast<IstreamType&>(rhs));
				_streambuf = std::move(rhs._streambuf);
				basic_ios<T_Char, T_CharTraits>::set_rdbuf(&_streambuf);
				rhs.basic_ios<T_Char, T_CharTraits>::set_rdbuf(&rhs._streambuf);
				return *this;
			}
			ConvertedIstreamBuf* rdbuf() const { return &_streambuf; }
		protected:
			ConvertedIstreamBuf _streambuf;
		};
		class ConvertedOstream final : public OstreamType {
		public:
			using char_type = T_Char;
			using traits_type = T_CharTraits;
			using int_type = typename T_CharTraits::int_type;
			using pos_type = typename T_CharTraits::pos_type;
			using off_type = typename T_CharTraits::off_type;
			explicit ConvertedOstream(StreambufType* streambuf_wrapped)
				: OstreamType(nullptr),
				_streambuf(streambuf_wrapped) {
				basic_ios<T_Char, T_CharTraits>::set_rdbuf(&_streambuf);
			}
			ConvertedOstream(const ConvertedOstream&) = delete;
			ConvertedOstream(ConvertedOstream&& rhs)
				: OstreamType(std::move(static_cast<OstreamType&>(rhs))),
				_streambuf(std::move(rhs._streambuf)) {
				basic_ios<T_Char, T_CharTraits>::set_rdbuf(&_streambuf);
				rhs.basic_ios<T_Char, T_CharTraits>::set_rdbuf(&rhs._streambuf);
			}
			virtual ~ConvertedOstream() override = default;
			ConvertedOstream& operator=(const ConvertedOstream&) = delete;
			ConvertedOstream& operator=(ConvertedOstream&& rhs) {
				static_cast<OstreamType&>(*this) = std::move(static_cast<OstreamType&>(rhs));
				_streambuf = std::move(rhs._streambuf);
				basic_ios<T_Char, T_CharTraits>::set_rdbuf(&_streambuf);
				rhs.basic_ios<T_Char, T_CharTraits>::set_rdbuf(&rhs._streambuf);
				return *this;
			}
			ConvertedOstreamBuf* rdbuf() const { return &_streambuf; }
		protected:
			ConvertedOstreamBuf _streambuf;
		};
	};

	template<>
	class StringConvert<char, std::char_traits<char>> {
	public:
		using CharType = char;
		using CharTraitsType = std::char_traits<char>;
		using StringViewType = std::basic_string_view<char, std::char_traits<char>>;
		template<typename T_Alloc = std::allocator<char>>
		using StringTypeTmpl = std::basic_string<char, std::char_traits<char>, T_Alloc>;
		using StreambufType = std::basic_streambuf<char, std::char_traits<char>>;
		using IstreamType = std::basic_istream<char, std::char_traits<char>>;
		using OstreamType = std::basic_ostream<char, std::char_traits<char>>;
		StringConvert(const StringConvert& rhs) = delete;
		StringConvert& operator=(const StringConvert& rhs) = delete;
		static constexpr char fromConstBasicChar(char ch_basic_from) {
			char ch_basic_to{};
			if (!std::char_traits<char>::eq(ch_basic_from, char{})) {
				ch_basic_to = mappingFromBasicChar<char>[static_cast<unsigned char>(ch_basic_from)];
				// Only basic characters are allowed.
				if (CharTraitsType::eq(ch_basic_to, CharType{})) { assert(false); abort(); }
			}
			return ch_basic_to;
		}
		template<std::size_t size_arr_basic_from>
		static constexpr std::array<char, size_arr_basic_from - 1> fromConstBasicCharArray(const char(&arr_basic_from)[size_arr_basic_from]) {
			std::array<char, size_arr_basic_from - 1> arr_str_to;
			for (std::size_t i = 0; i < size_arr_basic_from - 1; ++i) {
				char ch_basic_to{};
				if (!std::char_traits<char>::eq(arr_basic_from[i], char{})) {
					ch_basic_to = mappingFromBasicChar<char>[static_cast<unsigned char>(arr_basic_from[i])];
					// Only basic characters are allowed.
					if (CharTraitsType::eq(ch_basic_to, CharType{})) { assert(false); abort(); }
				}
				arr_str_to[i] = ch_basic_to;
			}
			return arr_str_to;
		}
		template<typename T_Alloc = std::allocator<char>, std::size_t size_arr_basic_from>
		static StringTypeTmpl<T_Alloc> fromConstBasicCharString(const char(&arr_basic_from)[size_arr_basic_from], const T_Alloc& alloc = T_Alloc()) {
			std::array<char, size_arr_basic_from - 1> arr_str_to = fromConstBasicCharArray(arr_basic_from);
			return StringTypeTmpl<T_Alloc>(arr_str_to.data(), arr_str_to.size(), alloc);
		}
		static std::size_t toMBCharRestartable(char(&mbch)[MB_LEN_MAX], const CharType& ch, std::mbstate_t& mbstate) {
			static_cast<void>(mbstate);
			mbch[0] = ch;
			return 1;
		}
		static std::size_t fromMBCharRestartable(CharType& ch, const char* p_mbs, std::size_t size_mbs, std::mbstate_t& mbstate) {
			static_cast<void>(mbstate);
			assert(p_mbs && size_mbs);
			ch = p_mbs[0];
			return 1;
		}
		template<typename T_Alloc = std::allocator<char>>
		static std::basic_string<char, std::char_traits<char>, T_Alloc> toMBStringRestartable(const StringViewType& strview, mbstate_t& mbstate) {
			static_cast<void>(mbstate);
			return std::basic_string<char, std::char_traits<char>, T_Alloc>(strview);
		}
		template<typename T_Alloc = std::allocator<char>>
		static StringTypeTmpl<T_Alloc> fromMBStringRestartable(const std::basic_string_view<char, std::char_traits<char>>& strview, mbstate_t& mbstate) {
			static_cast<void>(mbstate);
			return StringTypeTmpl<T_Alloc>(strview);
		}
		template<typename T_Alloc = std::allocator<char>>
		static std::basic_string<char, std::char_traits<char>, T_Alloc> toMBString(const StringViewType& strview) {
			mbstate_t mbstate{};
			return toMBStringRestartable(strview, mbstate);
		}
		template<typename T_Alloc = std::allocator<char>>
		static StringTypeTmpl<T_Alloc> fromMBString(const std::basic_string_view<char, std::char_traits<char>>& strview) {
			mbstate_t mbstate{};
			return fromMBStringRestartable(strview, mbstate);
		}
		class ConvertedIstream final : public IstreamType {
		public:
			using char_type = char;
			using traits_type = std::char_traits<char>;
			using int_type = typename std::char_traits<char>::int_type;
			using pos_type = typename std::char_traits<char>::pos_type;
			using off_type = typename std::char_traits<char>::off_type;
			explicit ConvertedIstream(StreambufType* streambuf_wrapped) : IstreamType(streambuf_wrapped) {}
			ConvertedIstream(const ConvertedIstream&) = delete;
			ConvertedIstream(ConvertedIstream&& rhs) : IstreamType(std::move(static_cast<IstreamType&>(rhs))) {
				rdbuf(rhs.rdbuf());
			}
			virtual ~ConvertedIstream() override = default;
			ConvertedIstream& operator=(const ConvertedIstream&) = delete;
			ConvertedIstream& operator=(ConvertedIstream&& rhs) {
				std::basic_ios<char_type, traits_type>::move(std::move(static_cast<std::basic_ios<char_type, traits_type>&>(rhs)));
				rdbuf(rhs.rdbuf());
				return *this;
			}
		};
		class ConvertedOstream final : public OstreamType {
		public:
			using char_type = char;
			using traits_type = std::char_traits<char>;
			using int_type = typename std::char_traits<char>::int_type;
			using pos_type = typename std::char_traits<char>::pos_type;
			using off_type = typename std::char_traits<char>::off_type;
			explicit ConvertedOstream(StreambufType* streambuf_wrapped) : OstreamType(streambuf_wrapped) {}
			ConvertedOstream(const ConvertedOstream&) = delete;
			ConvertedOstream(ConvertedOstream&& rhs) : OstreamType(std::move(static_cast<OstreamType&>(rhs))) {
				rdbuf(rhs.rdbuf());
			}
			virtual ~ConvertedOstream() override = default;
			ConvertedOstream& operator=(const ConvertedOstream&) = delete;
			ConvertedOstream& operator=(ConvertedOstream&& rhs) {
				std::basic_ios<char_type, traits_type>::move(std::move(static_cast<std::basic_ios<char_type, traits_type>&>(rhs)));
				rdbuf(rhs.rdbuf());
				return *this;
			}
		};
	};

	template<typename T_Char = char, typename T_CharTraits = std::char_traits<T_Char>>
	class ConvertedStdioStreams {
	public:
		using CharType = T_Char;
		using CharTraitsType = T_CharTraits;
		using StringConvertType = StringConvert<T_Char, T_CharTraits>;
		using StringViewType = std::basic_string_view<T_Char, T_CharTraits>;
		using StringType = std::basic_string<T_Char, T_CharTraits, std::allocator<T_Char>>;
		using StreambufType = std::basic_streambuf<T_Char, T_CharTraits>;
		using IstreamType = std::basic_istream<T_Char, T_CharTraits>;
		using OstreamType = std::basic_ostream<T_Char, T_CharTraits>;
		ConvertedStdioStreams()
			: _init_stdios(),
			convertedCin(std::cin.rdbuf()),
			convertedCout(std::cout.rdbuf()),
			convertedCerr(std::cerr.rdbuf()),
			convertedClog(std::clog.rdbuf()) {
			convertedCin.tie(&convertedCout);
			convertedCerr.setf(std::ios_base::unitbuf, std::ios_base::unitbuf);
			convertedCerr.tie(&convertedCout);
		}
		ConvertedStdioStreams(const ConvertedStdioStreams&) = delete;
		ConvertedStdioStreams(ConvertedStdioStreams&& rhs)
			: _init_stdios(),
			convertedCin(std::move(rhs.convertedCin)),
			convertedCout(std::move(rhs.convertedCout)),
			convertedCerr(std::move(rhs.convertedCerr)),
			convertedClog(std::move(rhs.convertedClog)) {
			convertedCin.tie(&convertedCout);
			convertedCerr.setf(std::ios_base::unitbuf, std::ios_base::unitbuf);
			convertedCerr.tie(&convertedCout);
		}
		virtual ~ConvertedStdioStreams() = default;
		ConvertedStdioStreams& operator=(const ConvertedStdioStreams&) = delete;
		ConvertedStdioStreams& operator=(ConvertedStdioStreams&& rhs) {
			convertedCin = std::move(rhs.convertedCin);
			convertedCout = std::move(rhs.convertedCout);
			convertedCerr = std::move(rhs.convertedCerr);
			convertedClog = std::move(rhs.convertedClog);
			convertedCin.tie(&convertedCout);
			convertedCerr.setf(std::ios_base::unitbuf, std::ios_base::unitbuf);
			convertedCerr.tie(&convertedCout);
			return *this;
		}
		const IstreamType& getCin() const { return convertedCin; }
		IstreamType& getCin() { return convertedCin; }
		const OstreamType& getCout() const { return convertedCout; }
		OstreamType& getCout() { return convertedCout; }
		const OstreamType& getCerr() const { return convertedCerr; }
		OstreamType& getCerr() { return convertedCerr; }
		const OstreamType& getClog() const { return convertedClog; }
		OstreamType& getClog() { return convertedClog; }
	protected:
		std::ios_base::Init _init_stdios;
		typename StringConvertType::ConvertedIstream convertedCin;
		typename StringConvertType::ConvertedOstream convertedCout;
		typename StringConvertType::ConvertedOstream convertedCerr;
		typename StringConvertType::ConvertedOstream convertedClog;
	};

	template<>
	class ConvertedStdioStreams<wchar_t, std::char_traits<wchar_t>> {
	public:
		using CharType = wchar_t;
		using CharTraitsType = std::char_traits<wchar_t>;
		using StringConvertType = StringConvert<wchar_t, std::char_traits<wchar_t>>;
		using StringViewType = std::basic_string_view<wchar_t, std::char_traits<wchar_t>>;
		using StringType = std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;
		using StreambufType = std::basic_streambuf<wchar_t, std::char_traits<wchar_t>>;
		using IstreamType = std::basic_istream<wchar_t, std::char_traits<wchar_t>>;
		using OstreamType = std::basic_ostream<wchar_t, std::char_traits<wchar_t>>;
		ConvertedStdioStreams() = default;
		ConvertedStdioStreams(const ConvertedStdioStreams&) = delete;
		ConvertedStdioStreams(ConvertedStdioStreams&&) = default;
		virtual ~ConvertedStdioStreams() = default;
		ConvertedStdioStreams& operator=(const ConvertedStdioStreams&) = delete;
		ConvertedStdioStreams& operator=(ConvertedStdioStreams&&) = default;
		const IstreamType& getCin() const { return std::wcin; }
		IstreamType& getCin() { return std::wcin; }
		const OstreamType& getCout() const { return std::wcout; }
		OstreamType& getCout() { return std::wcout; }
		const OstreamType& getCerr() const { return std::wcerr; }
		OstreamType& getCerr() { return std::wcerr; }
		const OstreamType& getClog() const { return std::wclog; }
		OstreamType& getClog() { return std::wclog; }
	protected:
		std::ios_base::Init _init_stdios;
	};

	template<>
	inline std::size_t StringConvert<wchar_t, std::char_traits<wchar_t>>::toMBCharRestartable(char(&mbch)[MB_LEN_MAX], const CharType& ch, std::mbstate_t& mbstate) {
		std::size_t result_mbconv = static_cast<std::size_t>(-1);
		errno_t errno_mbconv = wcrtomb_s(&result_mbconv, mbch, ch, &mbstate);
		if (errno_mbconv) result_mbconv = static_cast<std::size_t>(-1);
		return result_mbconv;
	}

	template<>
	inline std::size_t StringConvert<wchar_t, std::char_traits<wchar_t>>::fromMBCharRestartable(CharType& ch, const char* p_mbs, std::size_t size_mbs, std::mbstate_t& mbstate) {
		assert(p_mbs && size_mbs);
		return mbrtowc(&ch, p_mbs, size_mbs, &mbstate);
	}

#if \
defined(__cpp_char8_t) \
&& __cpp_char8_t >= 201811L\
&& defined(__cpp_lib_char8_t) \
&& __cpp_lib_char8_t >= 201907L \
&& false // TODO: Enable this block of code after there's support for std::c8rtomb in major compilers.
	template<>
	inline std::size_t StringConvert<char8_t, std::char_traits<char8_t>>::toMBCharRestartable(char(&mbch)[MB_LEN_MAX], const CharType& ch, std::mbstate_t& mbstate) {
		return std::c8rtomb(mbch, ch, &mbstate);
	}

	template<>
	inline std::size_t StringConvert<char8_t, std::char_traits<char8_t>>::fromMBCharRestartable(CharType& ch, const char* p_mbs, std::size_t size_mbs, std::mbstate_t& mbstate) {
		assert(p_mbs && size_mbs);
		return std::mbrtoc8(&ch, p_mbs, size_mbs, &mbstate);
	}
#endif

	template<>
	inline std::size_t StringConvert<char16_t, std::char_traits<char16_t>>::toMBCharRestartable(char(&mbch)[MB_LEN_MAX], const CharType& ch, std::mbstate_t& mbstate) {
		return std::c16rtomb(mbch, ch, &mbstate);
	}

	template<>
	inline std::size_t StringConvert<char16_t, std::char_traits<char16_t>>::fromMBCharRestartable(CharType& ch, const char* p_mbs, std::size_t size_mbs, std::mbstate_t& mbstate) {
		assert(p_mbs && size_mbs);
		return std::mbrtoc16(&ch, p_mbs, size_mbs, &mbstate);
	}

	template<>
	inline std::size_t StringConvert<char32_t, std::char_traits<char32_t>>::toMBCharRestartable(char(&mbch)[MB_LEN_MAX], const CharType& ch, std::mbstate_t& mbstate) {
		return std::c32rtomb(mbch, ch, &mbstate);
	}

	template<>
	inline std::size_t StringConvert<char32_t, std::char_traits<char32_t>>::fromMBCharRestartable(CharType& ch, const char* p_mbs, std::size_t size_mbs, std::mbstate_t& mbstate) {
		assert(p_mbs && size_mbs);
		return std::mbrtoc32(&ch, p_mbs, size_mbs, &mbstate);
	}
} //namespace TCLAP

#endif
