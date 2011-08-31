#pragma once

#ifdef __APPLE__

#include <libkern/OSAtomic.h>

inline bool cmp_exchange_barrier(void * old_value, void * new_value, void * volatile * value) {
	return OSAtomicCompareAndSwapPtrBarrier(old_value, new_value, value);
}

inline bool cmp_exchange(void * old_value, void * new_value, void * volatile * value) {
	return OSAtomicCompareAndSwapPtr(old_value, new_value, value);
}

#endif

template <class T>
inline bool cmp_exchange_barrier(T * const & old_value, T * const & new_value, T * volatile * value) {
	return cmp_exchange_barrier(
		static_cast<void*>(old_value),
		static_cast<void*>(new_value),
		reinterpret_cast<void * volatile *>(value)
	);
}

template <class T>
inline bool cmp_exchange(T * const & old_value, T * const & new_value, T * volatile * value) {
	return cmp_exchange(
		static_cast<void*>(old_value),
		static_cast<void*>(new_value),
		reinterpret_cast<void * volatile *>(value)
	);
}
