#pragma once

#include <libkern/OSAtomic.h>
#include <boost/optional.hpp>
#include <boost/utility.hpp>

#include <iostream>

inline bool cmp_exchange_barrier(void * old_value, void * new_value, void * volatile * value) {
	return OSAtomicCompareAndSwapPtrBarrier(old_value, new_value, value);
}

inline bool cmp_exchange(void * old_value, void * new_value, void * volatile * value) {
	return OSAtomicCompareAndSwapPtr(old_value, new_value, value);
}

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

template <class T>
class AsynchronousQueue : boost::noncopyable {
	struct Node {
		Node() : next(0) { }
		Node(T const & t) : value(t), next(0) { }

		boost::optional<T> value;
		Node * next;
	};

	public:
		AsynchronousQueue() :
			head_(new Node()), tail_(head_),
			free_head_(new Node()), free_tail_(free_head_)
		{ }

		bool shift(boost::optional<T> & out) {
			Node * first = 0;
			bool done = false;

			while (!done) {
				first = head_;
				if (head_ == tail_) { return false; }
				done = cmp_exchange(first, first->next, &head_);
			}

			out = first->next->value;
			free_node(first);
			return out;
		}

		void push(T const & t) {
			Node * next = allocate_node(t), * last = 0;
			bool done = false;

			while (!done) {
				last = tail_;
				if (tail_->next) { cmp_exchange(last, last->next, &tail_); }
				else { done = cmp_exchange_barrier(static_cast<Node*>(0), next, &last->next); }
			}

			cmp_exchange(last, last->next, &tail_);
		}

	private:
		Node * allocate_node(T const & t) {
			if (free_head_ == free_tail_) { return new Node(t); }
			Node * node = free_head_;

			if (cmp_exchange(node, node->next, &free_head_)) {
				node->value = t;
				node->next = 0;
			}
			else { node = new Node(t); }
			
			return node;
		}

		void free_node(Node * node) {
			node->next->value = boost::none;
			node->next = 0;

			Node * last = 0;
			bool done = false;

			while (!done) {
				last = free_tail_;
				if (free_tail_->next) { cmp_exchange(last, last->next, &free_tail_); }
				else { done = cmp_exchange_barrier(static_cast<Node*>(0), node, &last->next); }
			}

			cmp_exchange(last, last->next, &free_tail_);
		}

		Node * head_, * tail_;
		Node * free_head_, * free_tail_;
};
