#ifndef SVH_COMMON_CONSTRAINT_H__
#define SVH_COMMON_CONSTRAINT_H__
#pragma once

/*
	thank you once again, Stroustrup. <3
*/

template<class T, class B> struct derived_from {
	static void constraints(T* p) { B* pb = p; }
	derived_from() {
		void(*p)(T*) = constraints;
	}
};

#endif
