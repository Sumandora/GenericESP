#include "GenericESP/Element/Text.hpp"

#include "GenericESP/Element/Element.hpp"

#include "imgui.h"

#include <optional>
#include <string>

using namespace GenericESP;

std::optional<ImVec2> Text::draw(ImDrawList* draw_list, const EntityType* e, const std::string& text, const ImVec2& pos, const TextAlignment horizontal_alignment, const VerticalAlignment vertical_alignment) const
{
	if (text.empty())
		return std::nullopt;

	const float font_scale = this->get_font_scale(e);
	if (font_scale <= 0.0F)
		return std::nullopt;

	// Hack
	const float old_font_scale = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale *= font_scale;
	ImGui::PushFont(ImGui::GetFont());

	const ImVec2 size = ImGui::CalcTextSize(text.c_str());

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
		draw_list->AddText(ImVec2(position.x + shadow_offset, position.y + shadow_offset), get_shadow_color(e), text.c_str());
	}

	draw_list->AddText(position, get_font_color(e), text.c_str());

	ImGui::GetFont()->Scale = old_font_scale;
	ImGui::PopFont();

	return size;
}

[[nodiscard]] float Text::get_line_height(const EntityType* e) const
{
	return ImGui::GetTextLineHeight() * get_font_scale(e);
}
