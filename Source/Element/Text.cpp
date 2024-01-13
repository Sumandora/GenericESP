#include "GenericESP/Element/Text.hpp"

using namespace GenericESP;

Text::Text(ESP* base)
	: Element(base)
	, fontScale{ "Font scale", StaticConfig<float>{ 1.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, fontColor{ "Font color", StaticConfig<ImColor>{ { 1.0f, 1.0f, 1.0f, 1.0f }, base->createColorRenderer() } }
	, shadow{ "Shadow", StaticConfig<bool>{ true, base->createBoolRenderer() } }
	, shadowOffset{ "Shadow offset", StaticConfig<float>{ 1.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") }, [this] {
					   const ConfigurableValue<bool>& selected = shadow.getSelected();
					   return !selected.isStatic() || selected.getStaticConfig().thing;
				   } }
	, shadowColor{ "Shadow color", StaticConfig<ImColor>{ { 0.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer() }, [this] {
					  const ConfigurableValue<bool>& selected = shadow.getSelected();
					  return !selected.isStatic() || selected.getStaticConfig().thing;
				  } }
{
}

std::optional<ImVec2> Text::draw(ImDrawList* drawList, const void* e, const std::string& text, const ImVec2& pos, const TextAlignment horizontalAlignment, const VerticalAlignment verticalAlignment) const
{
	if (!enabled(e) || text.empty())
		return std::nullopt;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float fontScale = this->fontScale(e);
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

	if (shadow(e)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		const float shadowOffset = this->shadowOffset(e);
#pragma clang diagnostic pop
		drawList->AddText(ImVec2(position.x + shadowOffset, position.y + shadowOffset), shadowColor(e), text.c_str());
	}

	drawList->AddText(position, fontColor(e), text.c_str());

	ImGui::GetFont()->Scale = oldFontScale;
	ImGui::PopFont();

	return size;
}

[[nodiscard]] float Text::getLineHeight(const void* e) const
{
	return ImGui::GetTextLineHeight() * fontScale(e);
}

void Text::renderGui(const std::string& id)
{
	ImGui::PushID(id.c_str());
	enabled.renderGui();
	fontScale.renderGui();
	fontColor.renderGui();
	shadow.renderGui();
	shadowOffset.renderGui();
	shadowColor.renderGui();
	ImGui::PopID();
}