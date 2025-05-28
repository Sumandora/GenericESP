#include "GenericESP/Element/Flags.hpp"

#include "GenericESP/Element/Element.hpp"
#include "GenericESP/Element/SidedElement.hpp"
#include "GenericESP/Element/Text.hpp"
#include "GenericESP/Flag.hpp"
#include "GenericESP/UnionedRect.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

using namespace GenericESP;

ImVec2 Flags::draw_entry(ImDrawList* draw_list, const EntityType* e, ImRect& rect, const Flag& flag, float y_offset) const
{
	const float spacing = this->get_spacing(e);
	const std::string text = flag.compute_text(e);

	switch (get_side(e)) {
	case Side::TOP: {
		auto size = flag.draw(draw_list, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5F, rect.Min.y - spacing - y_offset }, TextAlignment::CENTERED, VerticalAlignment::ABOVE_POSITION);
		if (size.has_value())
			return { y_offset + size->y, size->y };
	}
	case Side::LEFT: {
		auto size = flag.draw(draw_list, e, text, { rect.Min.x - spacing, rect.Min.y + y_offset }, TextAlignment::RIGHT_BOUNDED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return size.value();
	}
	case Side::BOTTOM: {
		auto size = flag.draw(draw_list, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5F, rect.Max.y + spacing + y_offset }, TextAlignment::CENTERED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return { y_offset + size->y, size->y };
	}
	case Side::RIGHT: {
		auto size = flag.draw(draw_list, e, text, { rect.Max.x + spacing, rect.Min.y + y_offset }, TextAlignment::LEFT_BOUNDED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return size.value();
	}
	}
	return { 0.0F, 0.0F };
}

void Flags::draw(ImDrawList* draw_list, const EntityType* e, UnionedRect& unioned_rect) const
{
	ImRect& rect = choose_rect(e, unioned_rect);

	const float spacing = this->get_spacing(e);
	const float line_spacing = this->get_line_spacing(e);

	float biggest_offset = 0.0F;
	float y_offset = 0.0F;

	const Side side = get_side(e);

	auto process = [&](const Flag& flag) {
		const ImVec2 offset = draw_entry(draw_list, e, rect, flag, y_offset);
		biggest_offset = std::max(offset.x, biggest_offset);
		y_offset += offset.y * line_spacing;
	};

	const std::vector<Flag*>& flags = get_order(e);

	if (side == Side::TOP) // We render from bottom to top in this case, so reverse the flags to get the same order
		for (const auto* flag : std::ranges::views::reverse(flags))
			process(*flag);
	else
		for (const auto* flag : flags)
			process(*flag);

	biggest_offset += spacing;

	switch (side) {
	case Side::TOP:
		rect.Min.y -= biggest_offset;
		break;
	case Side::LEFT:
		rect.Min.x -= biggest_offset;
		break;
	case Side::BOTTOM:
		rect.Max.y += biggest_offset;
		break;
	case Side::RIGHT:
		rect.Max.x += biggest_offset;
		break;
	}
}
