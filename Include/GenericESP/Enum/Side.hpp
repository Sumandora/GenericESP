#ifndef GENERICESP_ENUM_SIDE_HPP
#define GENERICESP_ENUM_SIDE_HPP

#include <forward_list>
#include <list>
#include <string>
#include <unordered_map>

namespace GenericESP {
	enum class Side {
		TOP,
		LEFT,
		BOTTOM,
		RIGHT
	};

	inline std::list<std::string> sideLocalization{
		"Top",
		"Left",
		"Bottom",
		"Right"
	};

}

#endif
