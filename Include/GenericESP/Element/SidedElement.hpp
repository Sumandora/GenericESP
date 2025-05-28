#ifndef GENERICESP_ELEMENT_SIDEDELEMENT_HPP
#define GENERICESP_ELEMENT_SIDEDELEMENT_HPP

#include "Element.hpp"

#include "../UnionedRect.hpp"

#include "imgui_internal.h"

#include <cstdint>
#include <initializer_list>
#include <string>

namespace GenericESP {
	enum class Side : std::uint8_t {
		TOP,
		LEFT,
		BOTTOM,
		RIGHT
	};

	extern std::initializer_list<std::string> side_localization;

	struct SidedElement {
		virtual ~SidedElement() = default;

		[[nodiscard]] const ImRect& choose_rect(const EntityType* e, const UnionedRect& unioned_rect) const;
		[[nodiscard]] ImRect& choose_rect(const EntityType* e, UnionedRect& unioned_rect) const;

		GENERICESP_SETTING(Side, side);
	};

}

#endif
