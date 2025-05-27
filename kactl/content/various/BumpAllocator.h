/**
 * Author: Kirill Konovalov
 * Date: 2025-05
 * License: CC0
 * Source: Effective C++ for HFT course
 * Description: Static allocator
 */
#pragma once
#include "../../stress-tests/utilities/template.h"

static constexpr size_t STATIC_ALLOCATOR_BUFFER_SIZE = 500 * 1024 * 1024; // 500 MB
uint8_t static_allocator_buffer[STATIC_ALLOCATOR_BUFFER_SIZE]; size_t static_allocator_offset;
// ALIGNEMENT IS CRITICAL! DO NOT FORGET. 
template<class T> struct static_allocator {
	T *allocate(size_t n) {
		if (n > numeric_limits<size_t>::max() / sizeof(T)) { throw bad_alloc(); }
		size_t bytes = n * sizeof(T);
		size_t aligned_offset = (static_allocator_offset + alignof(T) - 1) & ~(alignof(T) - 1);
		if (aligned_offset + bytes > STATIC_ALLOCATOR_BUFFER_SIZE) { throw bad_alloc(); }
		T *allocated_ptr = reinterpret_cast<T *>(&static_allocator_buffer[aligned_offset]);
		static_allocator_offset = aligned_offset + bytes;
		return allocated_ptr;
	}
	template<class U, class... Args> void construct(U *p, Args &&...args) { new (p) U(forward<Args>(args)...); }
};