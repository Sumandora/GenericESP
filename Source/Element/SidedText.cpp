#include "GenericESP/Element/SidedText.hpp"

#include "GenericESP/Element/Element.hpp"
#include "GenericESP/Element/SidedElement.hpp"
#include "GenericESP/Element/Text.hpp"
#include "GenericESP/UnionedRect.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include <string>

using namespace GenericESP;

void SidedText::draw(ImDrawList* draw_list, const EntityType* e, const std::string& text, UnionedRect& unioned_rect) const
{
	ImRect& rect = choose_rect(e, unioned_rect);

	const float spacing = this->get_spacing(e);

	switch (get_side(e)) {
	case Side::TOP: {
		auto size = Text::draw(draw_list, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5F, rect.Min.y - spacing }, TextAlignment::CENTERED, VerticalAlignment::ABOVE_POSITION);
		if (size.has_value())
			rect.Min.y -= size->y + spacing;
		break;
	}
	case Side::LEFT: {
		auto size = Text::draw(draw_list, e, text, { rect.Min.x - spacing, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.5F }, TextAlignment::RIGHT_BOUNDED, VerticalAlignment::CENTERED);
		if (size.has_value())
			rect.Min.x -= size->x + spacing;
		break;
	}
	case Side::BOTTOM: {
		auto size = Text::draw(draw_list, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5F, rect.Max.y + spacing }, TextAlignment::CENTERED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			rect.Max.y += size->y + spacing;
		break;
	}
	case Side::RIGHT: {
		auto size = Text::draw(draw_list, e, text, { rect.Max.x + spacing, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.5F }, TextAlignment::LEFT_BOUNDED, VerticalAlignment::CENTERED);
		if (size.has_value())
			rect.Max.x += size->x + spacing;
		break;
	}
	}
}
