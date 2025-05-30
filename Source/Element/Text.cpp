#include "GenericESP/Element/Text.hpp"

#include "GenericESP/Element/Element.hpp"

#include "imgui.h"

#include <cfloat>
#include <optional>
#include <string>

using namespace GenericESP;

std::optional<ImVec2> Text::draw(ImDrawList* draw_list, const EntityType* e, const std::string& text, const ImVec2& pos, const TextAlignment horizontal_alignment, const VerticalAlignment vertical_alignment) const
{
	if (text.empty())
		return std::nullopt;

	const float font_size = this->get_font_size(e);
	if (font_size <= 0.0F)
		return std::nullopt;

	ImFont* font = this->get_font(e);

	const ImVec2 size = get_text_size(text, font, font_size);

	ImVec2 position(pos.x, pos.y);

	switch (horizontal_alignment) {
	case TextAlignment::LEFT_BOUNDED:
		break; // Well...
	case TextAlignment::CENTERED:
		position.x -= size.x / 2.0F;
		break;
	case TextAlignment::RIGHT_BOUNDED:
		position.x -= size.x;
		break;
	}

	switch (vertical_alignment) {
	case VerticalAlignment::BELOW_POSITION:
		break; // Well...
	case VerticalAlignment::CENTERED:
		position.y -= size.y / 2.0F;
		break;
	case VerticalAlignment::ABOVE_POSITION:
		position.y -= size.y;
		break;
	}

	if (get_shadow(e)) {
		const float shadow_offset = this->get_shadow_offset(e);
		draw_list->AddText(font, font_size, ImVec2(position.x + shadow_offset, position.y + shadow_offset), get_shadow_color(e), text.c_str());
	}

	draw_list->AddText(font, font_size, position, get_font_color(e), text.c_str());

	return size;
}

ImVec2 Text::get_text_size(const std::string& text, const EntityType* e) const
{
	ImFont* font = get_font(e);
	const float font_size = get_font_size(e);
	return get_text_size(text, font, font_size);
}

ImVec2 Text::get_text_size(const std::string& text, ImFont* font, float font_size)
{
	// This is an internal function, too bad.
	return font->CalcTextSizeA(font_size, FLT_MAX, 0.0F, text.c_str());
}
