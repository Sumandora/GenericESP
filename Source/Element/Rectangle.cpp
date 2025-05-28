#define IMGUI_DEFINE_MATH_OPERATORS

#include "GenericESP/Element/Rectangle.hpp"

#include "GenericESP/RendererFactory.hpp"

using namespace GenericESP;

ImDrawFlags Rectangle::getRoundingFlags(const EntityType* e) const
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

void Rectangle::draw(ImDrawList* drawList, const EntityType* e, UnionedRect& rect) const
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float rounding = this->get_rounding(e);

	const float thickness = this->get_thickness(e);
	const bool outlined = this->get_outlined(e);
	const float outlineThickness = this->get_outline_thickness(e);
#pragma clang diagnostic pop

	const float totalWidth = outlined ? std::max(thickness, outlineThickness) : thickness;
	const float halfWidth = totalWidth / 2;

	const ImVec2 min = rect.getMin();
	const ImVec2 max = rect.getMax();

	if (outlined)
		drawList->AddRect(min, max, get_outline_color(e), rounding, getRoundingFlags(e), outlineThickness);

	if (get_fill(e)) {
		const float halfThickness = thickness / 2.0f;
		const ImVec2 rectWidth{ halfThickness, halfThickness };
		drawList->AddRectFilled(min + rectWidth, max - rectWidth, get_fill_color(e), rounding, getRoundingFlags(e));
	}

	drawList->AddRect(min, max, get_color(e), rounding, getRoundingFlags(e), thickness);

	rect.expand(halfWidth);
}
