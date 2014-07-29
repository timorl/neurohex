#ifndef UI_OBSERVABLE_HPP
#define UI_OBSERVABLE_HPP

#include <boost/signals2.hpp>
#include <functional>

namespace ui 
{

	template <typename Derived>
		class Observable 
		{
			public:
				mutable boost::signals2::signal<void (Derived const &)> sigModified;
		};

}// namespace ui

#endif
