#pragma once

#include <math/Vec.hpp>

template <class T>
struct Rectangle {
	Rectangle() : top_(0), left_(0), bottom_(0), right_(0) { }

	Rectangle(T const & top, T const & left,
		T const & bottom, T const & right)
	:
		top_(top), left_(left),
		bottom_(bottom), right_(right)
	{ }
	
	T const & get_top() const { return top_; }
	void set_top(T const & top) { top_ = top; }

	T const & get_left() const { return left_; }
	void set_left(T const & left) { left_ = left; }

	T const & get_bottom() const { return bottom_; }
	void set_bottom() { return bottom_; }

	T const & get_right() const { return right_; }
	void set_right(T const & right) { right_ = right; }

	template <class U>
	bool contains(U const & x, U const & y) const {
		return left_ <= x && x < right_
			&& bottom_ <= y && y < top_;
	}

	template <class U>
	bool contains(Vec<2, U> const & v) const {
		return contains(v.x(), v.y());
	}

	template <class stream>
	friend stream & operator << (stream & out, Rectangle<T> const & rect) {
		return out << "Rect("
			<< rect.top_ << ", "
			<< rect.left_ << ", "
			<< rect.bottom_ << ", "
			<< rect.right_ << ")";
	}

	private:
		T top_, left_, bottom_, right_;
};
