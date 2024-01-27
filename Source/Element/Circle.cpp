#include "GenericESP/Element/Circle.hpp"

#include "GenericESP/Serialization/ImColorSerialization.hpp"

using namespace GenericESP;

Circle::Circle(ESP* base, std::string id, bool topLevel)
	: Element(base, std::move(id), topLevel)
	, color{ StaticConfig<ImColor>{ "Color", { 1.0f, 1.0f, 1.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor } }
	, radius{ StaticConfig<float>{ "Radius", 1.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, outlined{ StaticConfig<bool>{ "Outlined", true, base->createBoolRenderer() } }
	, outlineColor{ StaticConfig<ImColor>{ "Outline color", { 0.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor } }
	, outlineRadius{ StaticConfig<float>{ "Outline radius", 2.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
{
}

void Circle::draw(ImDrawList* drawList, const EntityType* e, const ImVec2& position) const
{
	if (!enabled(e))
		return;

	if (outlined(e))
		drawList->AddCircleFilled(position, outlineRadius(e), outlineColor(e));

	drawList->AddCircleFilled(position, radius(e), color(e));
}

void Circle::renderGui()
{
	ImGui::PushID(id.c_str());
	for (Renderable* r : std::initializer_list<Renderable*>{
			 &enabled, &color, &radius, &outlined,
			 &outlineColor, &outlineRadius })
		r->renderGui();
	ImGui::PopID();
}

SerializedTypeMap Circle::serialize() const
{
	SerializedTypeMap map;
	for (const MixableBase* mixable : std::initializer_list<const MixableBase*>{
			 &enabled, &color, &radius, &outlined,
			 &outlineColor, &outlineRadius })
		mixable->serialize(map);
	return map;
}

void Circle::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &enabled, &color, &radius, &outlined,
			 &outlineColor, &outlineRadius })
		mixable->deserializeFromParent(map);
}
