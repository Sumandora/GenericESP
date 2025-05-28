#define IMGUI_DEFINE_MATH_OPERATORS
#include "GenericESP/Element/Element.hpp"

#include "GenericESP/Element/Rectangle.hpp"
#include "GenericESP/UnionedRect.hpp"

#include "imgui.h"

#include <algorithm>

using namespace GenericESP;

ImDrawFlags Rectangle::get_rounding_flags(const EntityType* e) const
{
	int flags = ImDrawFlags_RoundCornersNone;

	if (get_rounding_top_left(e))
		flags |= ImDrawFlags_RoundCornersTopLeft;

	if (get_rounding_top_right(e))
		flags |= ImDrawFlags_RoundCornersTopRight;

	if (get_rounding_bottom_left(e))
		flags |= ImDrawFlags_RoundCornersBottomLeft;

	if (get_rounding_bottom_right(e))
		flags |= ImDrawFlags_RoundCornersBottomRight;

	return flags;
}

void Rectangle::draw(ImDrawList* draw_list, const EntityType* e, UnionedRect& rect) const
{
	const float rounding = this->get_rounding(e);

	const float thickness = this->get_thickness(e);
	const bool outlined = this->get_outlined(e);
	const float outline_thickness = this->get_outline_thickness(e);

	const float total_width = outlined ? std::max(thickness, outline_thickness) : thickness;
	const float half_width = total_width / 2;

	const ImVec2 min = rect.get_min();
	const ImVec2 max = rect.get_max();

	if (outlined)
		draw_list->AddRect(min, max, get_outline_color(e), rounding, get_rounding_flags(e), outline_thickness);

	if (get_fill(e)) {
		const float half_thickness = thickness / 2.0F;
		const ImVec2 rect_width{ half_thickness, half_thickness };
		draw_list->AddRectFilled(min + rect_width, max - rect_width, get_fill_color(e), rounding, get_rounding_flags(e));
	}

	draw_list->AddRect(min, max, get_color(e), rounding, get_rounding_flags(e), thickness);

	rect.expand(half_width);
}
