#pragma once

#include <algorithm>

namespace std {
	template <class, class, class> class set;
	template <class, class, class> class multiset;
	template <class, class, class, class> class map;
	template <class, class, class, class> class multimap;
}

namespace detail {
	struct erase_if_remove_if_impl {
		template <class Container, class F>
		static inline void call(Container & c, F & f) {
			c.erase(std::remove_if(c.begin(), c.end(), f), c.end());
		}

		template <class Container, class F>
		static inline void call(Container & c, F const & f) {
			c.erase(std::remove_if(c.begin(), c.end(), f), c.end());
		}
	};

	struct erase_if_stable_impl {
		template <class Container, class F>
		static inline void call(Container & c, F & f) {
			typedef typename Container::iterator iterator;
			for (iterator it = c.begin(); it != c.end(); ) {
				if (f(*it)) { c.erase(it++); }
				else { ++it; }
			}
		}

		template <class Container, class F>
		static inline void call(Container & c, F const & f) {
			typedef typename Container::iterator iterator;
			for (iterator it = c.begin(); it != c.end(); ) {
				if (f(*it)) { c.erase(it++); }
				else { ++it; }
			}
		}
	};

	template <class Container> struct erase_if_impl : erase_if_remove_if_impl { };

	template <class A, class B, class C> struct erase_if_impl<std::set<A, B, C> >
		: erase_if_stable_impl { };
	
	template <class A, class B, class C> struct erase_if_impl<std::multiset<A, B, C> >
		: erase_if_stable_impl { };
	
	template <class A, class B, class C, class D> struct erase_if_impl<std::map<A, B, C, D> >
		: erase_if_stable_impl { };
	
	template <class A, class B, class C, class D> struct erase_if_impl<std::multimap<A, B, C, D> >
		: erase_if_stable_impl { };
}

template <class Container, class F>
inline void erase_if(Container & c, F & f) {
	detail::erase_if_impl<Container>::call(c, f);
}

template <class Container, class F>
inline void erase_if(Container & c, F const & f) {
	detail::erase_if_impl<Container>::call(c, f);
}
