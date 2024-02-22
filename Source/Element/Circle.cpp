#include "GenericESP/Element/Circle.hpp"

#include "GenericESP/RendererFactory.hpp"
#include "GenericESP/Serialization/ImColorSerialization.hpp"

using namespace GenericESP;

Circle::Circle(ESP* base, std::string id, bool topLevel)
	: Element(base, std::move(id), topLevel)
	, color{ StaticConfig<ImColor>{ "Color", { 1.0f, 1.0f, 1.0f, 1.0f }, rendererFactory.createColorRenderer(), serializeImColor, deserializeImColor } }
	, radius{ StaticConfig<float>{ "Radius", 1.0f, rendererFactory.createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, outlined{ StaticConfig<bool>{ "Outlined", true, rendererFactory.createBoolRenderer() } }
	, outlineColor{ StaticConfig<ImColor>{ "Outline color", { 0.0f, 0.0f, 0.0f, 1.0f }, rendererFactory.createColorRenderer(), serializeImColor, deserializeImColor } }
	, outlineThickness{ StaticConfig<float>{ "Outline thickness", 1.0f, rendererFactory.createFloatRenderer(0.0f, 10.0f, "%.2f") } }
{
}

void Circle::draw(ImDrawList* drawList, const EntityType* e, const ImVec2& position) const
{
	if (!enabled(e))
		return;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float radius = this->radius(e);
#pragma clang diagnostic pop

	if (outlined(e))
		drawList->AddCircleFilled(position, radius + outlineThickness(e), outlineColor(e));

	drawList->AddCircleFilled(position, radius, color(e));
}

void Circle::renderGui()
{
	ImGui::PushID(id.c_str());
	for (Renderable* r : std::initializer_list<Renderable*>{
			 &enabled, &color, &radius, &outlined,
			 &outlineColor, &outlineThickness })
		r->renderGui();
	ImGui::PopID();
}

SerializedTypeMap Circle::serialize() const
{
	SerializedTypeMap map;
	for (const MixableBase* mixable : std::initializer_list<const MixableBase*>{
			 &enabled, &color, &radius, &outlined,
			 &outlineColor, &outlineThickness })
		mixable->serialize(map);
	return map;
}

void Circle::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &enabled, &color, &radius, &outlined,
			 &outlineColor, &outlineThickness })
		mixable->deserializeFromParent(map);
}
