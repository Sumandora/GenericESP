#include "GenericESP/Element/Text.hpp"

using namespace GenericESP;

std::optional<ImVec2> Text::draw(ImDrawList* drawList, const EntityType* e, const std::string& text, const ImVec2& pos, const TextAlignment horizontalAlignment, const VerticalAlignment verticalAlignment) const
{
	if (text.empty())
		return std::nullopt;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float fontScale = this->get_font_scale(e);
#pragma clang diagnostic pop
	if (fontScale <= 0.0f)
		return std::nullopt;

	// Hack
	const float oldFontScale = ImGui::GetFont()->Scale;
	ImGui::GetFont()->Scale *= fontScale;
	ImGui::PushFont(ImGui::GetFont());

	const ImVec2 size = ImGui::CalcTextSize(text.c_str());

	ImVec2 position(pos.x, pos.y);

	switch (horizontalAlignment) {
	case TextAlignment::LEFT_BOUNDED:
		break; // Well...
	case TextAlignment::CENTERED:
		position.x -= size.x / 2.0f;
		break;
	case TextAlignment::RIGHT_BOUNDED:
		position.x -= size.x;
		break;
	}

	switch (verticalAlignment) {
	case VerticalAlignment::BELOW_POSITION:
		break; // Well...
	case VerticalAlignment::CENTERED:
		position.y -= size.y / 2.0f;
		break;
	case VerticalAlignment::ABOVE_POSITION:
		position.y -= size.y;
		break;
	}

	if (get_shadow(e)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		const float shadowOffset = this->get_shadow_offset(e);
#pragma clang diagnostic pop
		drawList->AddText(ImVec2(position.x + shadowOffset, position.y + shadowOffset), get_shadow_color(e), text.c_str());
	}

	drawList->AddText(position, get_font_color(e), text.c_str());

	ImGui::GetFont()->Scale = oldFontScale;
	ImGui::PopFont();

	return size;
}

[[nodiscard]] float Text::getLineHeight(const EntityType* e) const
{
	return ImGui::GetTextLineHeight() * get_font_scale(e);
}
