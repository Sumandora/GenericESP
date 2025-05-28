#include "GenericESP/Element/SidedText.hpp"

using namespace GenericESP;

void SidedText::draw(ImDrawList* drawList, const EntityType* e, const std::string& text, UnionedRect& unionedRect) const
{
	ImRect& rect = chooseRect(e, unionedRect);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float spacing = this->get_spacing(e);
#pragma clang diagnostic pop

	switch (get_side(e)) {
	case Side::TOP: {
		auto size = Text::draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Min.y - spacing }, TextAlignment::CENTERED, VerticalAlignment::ABOVE_POSITION);
		if (size.has_value())
			rect.Min.y -= size->y + spacing;
		break;
	}
	case Side::LEFT: {
		auto size = Text::draw(drawList, e, text, { rect.Min.x - spacing, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.5f }, TextAlignment::RIGHT_BOUNDED, VerticalAlignment::CENTERED);
		if (size.has_value())
			rect.Min.x -= size->x + spacing;
		break;
	}
	case Side::BOTTOM: {
		auto size = Text::draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Max.y + spacing }, TextAlignment::CENTERED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			rect.Max.y += size->y + spacing;
		break;
	}
	case Side::RIGHT: {
		auto size = Text::draw(drawList, e, text, { rect.Max.x + spacing, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.5f }, TextAlignment::LEFT_BOUNDED, VerticalAlignment::CENTERED);
		if (size.has_value())
			rect.Max.x += size->x + spacing;
		break;
	}
	}
}
