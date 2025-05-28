#ifndef GENERICESP_ELEMENT_CIRCLE_HPP
#define GENERICESP_ELEMENT_CIRCLE_HPP

#include "Element.hpp"

#include "imgui.h"

namespace GenericESP {

	struct Circle {
		virtual ~Circle() = default;

		void draw(ImDrawList* draw_list, const EntityType* e, const ImVec2& position) const;

		GENERICESP_SETTING(ImColor, color);
		GENERICESP_SETTING(float, radius);
		GENERICESP_SETTING(bool, outlined);
		GENERICESP_SETTING(ImColor, outline_color);
		GENERICESP_SETTING(float, outline_radius);
	};

}

#endif
