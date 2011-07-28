#pragma once

template <class T>
struct ScalarType {
	typedef typename T::ScalarType type;
};
