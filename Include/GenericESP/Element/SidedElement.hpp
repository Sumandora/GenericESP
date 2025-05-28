#ifndef GENERICESP_ELEMENT_SIDEDELEMENT_HPP
#define GENERICESP_ELEMENT_SIDEDELEMENT_HPP

#include "../UnionedRect.hpp"
#include "Element.hpp"

#include <initializer_list>
#include <string>

namespace GenericESP {
	enum class Side {
		TOP,
		LEFT,
		BOTTOM,
		RIGHT
	};

	extern std::initializer_list<std::string> sideLocalization;

	struct SidedElement {
		[[nodiscard]] const ImRect& chooseRect(const EntityType* e, const UnionedRect& unionedRect) const;
		[[nodiscard]] ImRect& chooseRect(const EntityType* e, UnionedRect& unionedRect) const;

		GENERICESP_SETTING(Side, side);
	};

}

#endif
