#ifndef UI_OBSERVABLE_HPP
#define UI_OBSERVABLE_HPP

#include <boost/signals2.hpp>
#include <functional>

namespace ui 
{

	/**
		* @brief Inherit this if you want your class to be observable by UI elements.
		*/
	template <typename Derived>
		class Observable 
		{
			public:
				/**
					* @brief A function being ran every time the inheriting object is modified.
					* 
					* This function should be provided by the observing object and will be called
					* every time the inheriting object is modified in any way.
					* The call will usually pass a reference to the modified object as an argument.
					*/
				mutable boost::signals2::signal<void (Derived const &)> sigModified;
		};

}// namespace ui

#endif
