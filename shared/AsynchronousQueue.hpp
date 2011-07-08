#pragma once

#include <libkern/OSAtomic.h>
#include <boost/optional.hpp>

inline bool cmp_exchnage_barrier(void * old_value, void * new_value, void * volatile * value) {
	return OSAtomicCompareAndSwapPtrBarrier(old_value, new_value, value);
}

inline bool cmp_exchange(void * old_value, void * new_value, void * volatile * value) {
	return OSAtomicCompareAndSwapPtr(old_value, new_value, value);
}

template <class T>
class AsynchronousQueue {
	struct Node {
		Node() : next(0) { }
		Node(T const & t) : value(t), next(0) { }

		boost::optional<T> value;
		Node * next;
	};

	public:
		AsynchronousQueue() :
			head_(new Node()), tail_(head_) { }

		void shift(boost::optional<T> & out) {
			Node * first = 0;
			bool done = false;

			while (!done) {
				first = head_;
				if (!head_->next) { return; }
				if (first == head_) { done = cmp_exchange(first, first->next, &head_); }
			}

			out = first->next->value;
			// NOTE: We probably shouldn't just leak the Node...
		}

		void push(T const & t) {
			Node * next = new Node(t), * last = 0;
			bool done = false;

			while (!done) {
				last = tail_;
				if (tail_->next ) {
					if (last == tail_) { cmp_exchange(last, last->next, &tail_); }
					continue;
				}
				done = cmp_exchange_barrier(0, next, &last->next);
			}

			cmp_exchange(last, last->next, &tail_);
		}

	private:
		Node * head_, * tail_;
};
