#ifndef GENERICESP_ENUM_SIDE_HPP
#define GENERICESP_ENUM_SIDE_HPP

#include <string>
#include <unordered_map>

namespace GenericESP {

	enum class Side {
		TOP,
		LEFT,
		BOTTOM,
		RIGHT
	};

	inline std::unordered_map<Side, std::string> sideLocalization{
		{ Side::TOP, "Top" },
		{ Side::LEFT, "Left" },
		{ Side::BOTTOM, "Bottom" },
		{ Side::RIGHT, "Right" }
	};

}

#endif
