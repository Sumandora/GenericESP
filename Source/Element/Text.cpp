#include "GenericESP/Element/Text.hpp"

#include "GenericESP/RendererFactory.hpp"
#include "GenericESP/Serialization/ImColorSerialization.hpp"

using namespace GenericESP;

Text::Text(ESP* base, std::string id, bool topLevel)
	: Element(base, std::move(id), topLevel)
	, fontScale{ StaticConfig<float>{ "Font scale", 1.0f, rendererFactory->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, fontColor{ StaticConfig<ImColor>{ "Font color", { 1.0f, 1.0f, 1.0f, 1.0f }, rendererFactory->createColorRenderer(), serializeImColor, deserializeImColor } }
	, shadow{ StaticConfig<bool>{ "Shadow", true, rendererFactory->createBoolRenderer() } }
	, shadowOffset{ StaticConfig<float>{ "Shadow offset", 1.0f, rendererFactory->createFloatRenderer(0.0f, 10.0f, "%.2f") }, [this] {
					   const ConfigurableValue<bool>& selected = shadow.getSelected();
					   return !selected.isStatic() || selected.getStaticConfig().thing;
				   } }
	, shadowColor{ StaticConfig<ImColor>{ "Shadow color", { 0.0f, 0.0f, 0.0f, 1.0f }, rendererFactory->createColorRenderer(), serializeImColor, deserializeImColor }, [this] {
					  const ConfigurableValue<bool>& selected = shadow.getSelected();
					  return !selected.isStatic() || selected.getStaticConfig().thing;
				  } }
{
}

std::optional<ImVec2> Text::draw(ImDrawList* drawList, const EntityType* e, const std::string& text, const ImVec2& pos, const TextAlignment horizontalAlignment, const VerticalAlignment verticalAlignment) const
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

[[nodiscard]] float Text::getLineHeight(const EntityType* e) const
{
	return ImGui::GetTextLineHeight() * fontScale(e);
}

void Text::renderGui()
{
	ImGui::PushID(id.c_str());
	for (Renderable* r : std::initializer_list<Renderable*>{
			 &enabled, &fontScale, &fontColor, &shadow,
			 &shadowOffset, &shadowColor })
		r->renderGui();
	ImGui::PopID();
}

SerializedTypeMap Text::serialize() const
{
	SerializedTypeMap map;
	for (const MixableBase* mixable : std::initializer_list<const MixableBase*>{
			 &enabled, &fontScale, &fontColor, &shadow,
			 &shadowOffset, &shadowColor })
		mixable->serialize(map);
	return map;
}

void Text::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &enabled, &fontScale, &fontColor, &shadow,
			 &shadowOffset, &shadowColor })
		mixable->deserializeFromParent(map);
}