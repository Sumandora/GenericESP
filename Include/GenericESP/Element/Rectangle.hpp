#ifndef GENERICESP_ELEMENT_RECTANGLE_HPP
#define GENERICESP_ELEMENT_RECTANGLE_HPP

#include "../UnionedRect.hpp"
#include "Element.hpp"

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

		ImDrawFlags getRoundingFlags(const EntityType* e) const;

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& rect) const;
	};

}

#endif
