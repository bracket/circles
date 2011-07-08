#pragma once

#include <algorithm>

//NOTE: These could probably be moved somewhere more useful.
template <class InIt, class OutIt>
inline void assign(OutIt out, OutIt out_end, InIt in, InIt in_end) {
	for (; in != in_end && out != out_end; *out++ = *in++) { }
}

template <class InIt, class OutIt>
inline void plus_eq(OutIt out, OutIt out_end, InIt in, InIt in_end) {
	for (; in != in_end && out != out_end; *out++ += *in++) { }
}

template <class InIt, class OutIt>
inline void minus_eq(OutIt out, OutIt out_end, InIt in, InIt in_end) {
	for (; in != in_end && out != out_end; *out++ -= *in++) { }
}

template <class InIt, class OutIt>
inline void times_eq(OutIt out, OutIt out_end, InIt in, InIt in_end) {
	for (; in != in_end && out != out_end; *out++ *= *in++) { }
}

template <class InIt, class OutIt>
inline void div_eq(OutIt out, OutIt out_end, InIt in, InIt in_end) {
	for (; in != in_end && out != out_end; *out++ /= *in++) { }
}

// NOTE: Don't rely on sample_count_ and channel_count_ remaining template parameters.
// We may need dynamically sized blocks at some point.
template <class T, int sample_count_, int channel_count_>
class Block {
	typedef T channel_storage[sample_count_];
	typedef channel_storage sample_storage[channel_count_];

	public:
		typedef T value_type;

		typedef value_type * pointer;
		typedef value_type const * const_pointer;

		typedef pointer iterator;
		typedef const_pointer const_iterator;

		int channel_count() const { return channel_count_; }
		int sample_count() const { return sample_count_; }
		int size() const { return channel_count_ * sample_count_; }

		iterator begin() { return storage_; }
		const_iterator begin() const { return storage_; }

		iterator end() { return storage_ + size(); }
		const_iterator end() const { return storage_ + size(); }

		iterator channel_begin(int channel) { storage_[channel]; }
		const_iterator channel_begin(int channel) const { return storage_[channel]; }

		iterator channel_end(int channel) { return storage_[channel] + sample_count_; }
		const_iterator channel_end(int channel) const { return storage_[channel] + sample_count_; }

		template <class InIt>
		void assign_channel(int channel, InIt begin, InIt end) {
			assign(channel_begin(channel), channel_end(channel), begin, end);
		}

		template <class U>
		Block & operator += (Block<U, sample_count_, channel_count_> const & right) {
			plus_eq(begin(), end(), right.begin(), right.end());
			return *this;
		}

		template <class U>
		Block & operator -= (Block<U, sample_count_, channel_count_> const & right) {
			minus_eq(begin(), end(), right.begin(), right.end());
			return *this;
		}

		template <class U>
		Block & operator *= (Block<U, sample_count_, channel_count_> const & right) {
			minus_eq(begin(), end(), right.begin(), right.end());
			return *this;
		}

		template <class U>
		Block & operator /= (Block<U, sample_count_, channel_count_> const & right) {
			div_eq(begin(), end(), right.begin(), right.end());
			return *this;
		}

	private:
		sample_storage storage_;
};
