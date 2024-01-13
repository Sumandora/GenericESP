#include "GenericESP/Element/SidedText.hpp"

using namespace GenericESP;

SidedText::SidedText(ESP* base)
	: SidedElement(base, Side::TOP)
	, spacing{ "Spacing", StaticConfig<float>{ 1.0f, base->createFloatRenderer(0.0, 10.0f, "%.2f") } }
	, textElement(base)
{
}

void SidedText::draw(ImDrawList* drawList, const void* e, const std::string& text, UnionedRect& unionedRect) const
{
	if (!enabled(e))
		return;

	ImRect& rect = chooseRect(e, unionedRect);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float spacing = this->spacing(e);
#pragma clang diagnostic pop

	switch (side(e)) {
	case Side::TOP: {
		auto size = textElement.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Min.y - spacing }, TextAlignment::CENTERED, VerticalAlignment::ABOVE_POSITION);
		if (size.has_value())
			rect.Min.y -= size->y + spacing;
		break;
	}
	case Side::LEFT: {
		auto size = textElement.draw(drawList, e, text, { rect.Min.x - spacing, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.5f }, TextAlignment::RIGHT_BOUNDED, VerticalAlignment::CENTERED);
		if (size.has_value())
			rect.Min.x -= size->x + spacing;
		break;
	}
	case Side::BOTTOM: {
		auto size = textElement.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Max.y + spacing }, TextAlignment::CENTERED, VerticalAlignment::BELOW_POSITION);
		if (size.has_value())
			rect.Max.y += size->y + spacing;
		break;
	}
	case Side::RIGHT: {
		auto size = textElement.draw(drawList, e, text, { rect.Max.x + spacing, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.5f }, TextAlignment::LEFT_BOUNDED, VerticalAlignment::CENTERED);
		if (size.has_value())
			rect.Max.x += size->x + spacing;
		break;
	}
	}
}

void SidedText::renderGui(const std::string& id)
{
	ImGui::PushID(id.c_str());
	enabled.renderGui();
	side.renderGui();
	spacing.renderGui();
	textElement.renderGui("Text");
	ImGui::PopID();
}
