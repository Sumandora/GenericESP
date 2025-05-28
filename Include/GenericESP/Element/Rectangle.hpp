#ifndef GENERICESP_ELEMENT_RECTANGLE_HPP
#define GENERICESP_ELEMENT_RECTANGLE_HPP

#include "../UnionedRect.hpp"
#include "Element.hpp"
#include "imgui.h"

namespace GenericESP {

	struct Rectangle {
		GENERICESP_SETTING(ImColor, color);
		GENERICESP_SETTING(float, rounding);

		GENERICESP_SETTING(bool, rounding_top_left);
		GENERICESP_SETTING(bool, rounding_top_right);
		GENERICESP_SETTING(bool, rounding_bottom_left);
		GENERICESP_SETTING(bool, rounding_bottom_right);

		GENERICESP_SETTING(float, thickness);
		GENERICESP_SETTING(bool, outlined);
		GENERICESP_SETTING(ImColor, outline_color);
		GENERICESP_SETTING(float, outline_thickness);
		GENERICESP_SETTING(bool, fill);
		GENERICESP_SETTING(ImColor, fill_color);

		virtual ~Rectangle() = default;

		ImDrawFlags get_rounding_flags(const EntityType* e) const;

		void draw(ImDrawList* draw_list, const EntityType* e, UnionedRect& rect) const;
	};

}

#endif
