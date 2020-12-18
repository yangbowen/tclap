// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  UseAllocatorBase.h
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


#ifndef TCLAP_USEALLOCATORBASE_H
#define TCLAP_USEALLOCATORBASE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <type_traits>
#include <memory>

namespace TCLAP {
	template<typename T_Alloc>
	class UseAllocatorBase;

	template<typename T_Alloc>
	class UseAllocatorBase {
	public:
		using AllocatorType = T_Alloc;
		using AllocatorTraitsType = std::allocator_traits<T_Alloc>;
		template<typename T_Value>
		using AllocatorTypeTmpl = typename std::allocator_traits<AllocatorType>::template rebind_alloc<T_Value>;
		template<typename T_Value>
		using AllocatorTraitsTypeTmpl = typename std::allocator_traits<AllocatorType>::template rebind_traits<T_Value>;
		explicit UseAllocatorBase(const AllocatorType& alloc) noexcept : _alloc(alloc) {}
		UseAllocatorBase(const UseAllocatorBase& rhs) noexcept : _alloc(AllocatorTraitsType::select_on_container_copy_construction(rhs._alloc)) {}
		UseAllocatorBase(UseAllocatorBase&& rhs) noexcept : _alloc(::std::move(rhs._alloc)) {}
		UseAllocatorBase& operator=(const UseAllocatorBase& rhs) noexcept {
			if constexpr (AllocatorTraitsType::propagate_on_container_copy_assignment::value) {
				this->getAlloc() = rhs.getAlloc();
			}
			return *this;
		}
		UseAllocatorBase& operator=(UseAllocatorBase&& rhs) noexcept {
			if constexpr (AllocatorTraitsType::propagate_on_container_move_assignment::value) {
				this->getAlloc() = ::std::move(rhs.getAlloc());
			}
			return *this;
		}
		const AllocatorType& getAlloc() const noexcept { return this->_alloc; }
		AllocatorType& getAlloc() noexcept { return this->_alloc; }
		template<typename T_Value>
		const AllocatorTypeTmpl<T_Value> rebindAlloc() const noexcept { return AllocatorTypeTmpl<T_Value>(this->getAlloc()); }
	private:
		AllocatorType _alloc;
	};
	template<typename T_Alloc>
	inline void swap(UseAllocatorBase<T_Alloc>& lhs, UseAllocatorBase<T_Alloc>& rhs) noexcept {
		if constexpr (UseAllocatorBase<T_Alloc>::AllocatorTraitsType::propagate_on_container_swap::value) {
			swap(lhs.getAlloc(), rhs.getAlloc());
		}
	}

	template<typename T_Alloc>
	explicit UseAllocatorBase(const T_Alloc& alloc) -> UseAllocatorBase<T_Alloc>;
} //namespace TCLAP

#endif
