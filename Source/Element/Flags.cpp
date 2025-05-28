#include "GenericESP/Element/Flags.hpp"

#include "GenericESP/RendererFactory.hpp"

using namespace GenericESP;

ImVec2 Flags::drawEntry(ImDrawList* drawList, const EntityType* e, ImRect& rect, const Flag& flag, float yOffset) const
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float spacing = this->get_spacing(e);
#pragma clang diagnostic pop
	const std::string text = flag.computeText(e);

	switch (get_side(e)) {
	case Side::TOP: {
		auto size = flag.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Min.y - spacing - yOffset }, TextAlignment::CENTERED, VerticalAlignment::ABOVE_POSITION);
		if (size.has_value())
			return { yOffset + size->y, size->y };
	}
	case Side::LEFT: {
		auto size = flag.draw(drawList, e, text, { rect.Min.x - spacing, rect.Min.y + yOffset }, TextAlignment::RIGHT_BOUNDED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return size.value();
	}
	case Side::BOTTOM: {
		auto size = flag.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Max.y + spacing + yOffset }, TextAlignment::CENTERED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return { yOffset + size->y, size->y };
	}
	case Side::RIGHT: {
		auto size = flag.draw(drawList, e, text, { rect.Max.x + spacing, rect.Min.y + yOffset }, TextAlignment::LEFT_BOUNDED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			return size.value();
	}
	}
	return { 0.0f, 0.0f };
}

void Flags::draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const
{
	ImRect& rect = chooseRect(e, unionedRect);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float spacing = this->get_spacing(e);
	const float lineSpacing = this->get_line_spacing(e);
#pragma clang diagnostic pop

	float biggestOffset = 0.0f;
	float yOffset = 0.0f;

	Side side = get_side(e);

	auto process = [&](const Flag& flag) {
		ImVec2 offset = drawEntry(drawList, e, rect, flag, yOffset);
		if (offset.x > biggestOffset)
			biggestOffset = offset.x;
		yOffset += offset.y * lineSpacing;
	};

	const std::vector<Flag*>& flags = get_order(e);

	if (side == Side::TOP) // We render from bottom to top in this case, so reverse the flags to get the same order
		for (const auto* flag : std::ranges::views::reverse(flags))
			process(*flag);
	else
		for (const auto* flag : flags)
			process(*flag);

	biggestOffset += spacing;

	switch (side) {
	case Side::TOP:
		rect.Min.y -= biggestOffset;
		break;
	case Side::LEFT:
		rect.Min.x -= biggestOffset;
		break;
	case Side::BOTTOM:
		rect.Max.y += biggestOffset;
		break;
	case Side::RIGHT:
		rect.Max.x += biggestOffset;
		break;
	}
}

