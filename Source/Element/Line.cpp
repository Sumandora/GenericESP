#include "GenericESP/Element/Line.hpp"

#include "GenericESP/RendererFactory.hpp"
#include "GenericESP/Serialization/ImColorSerialization.hpp"

using namespace GenericESP;

Line::Line(ESP* base, std::string id, bool topLevel)
	: Element(base, std::move(id), topLevel)
	, color{ StaticConfig<ImColor>{ "Color", { 1.0f, 1.0f, 1.0f, 1.0f }, rendererFactory->createColorRenderer(), serializeImColor, deserializeImColor } }
	, thickness{ StaticConfig<float>{ "Thickness", 1.0f, rendererFactory->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, outlined{ StaticConfig<bool>{ "Outlined", true, rendererFactory->createBoolRenderer() } }
	, outlineColor{ StaticConfig<ImColor>{ "Outline color", { 0.0f, 0.0f, 0.0f, 1.0f }, rendererFactory->createColorRenderer(), serializeImColor, deserializeImColor } }
	, outlineThickness{ StaticConfig<float>{ "Outline thickness", 2.0f, rendererFactory->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
{
}

void Line::draw(ImDrawList* drawList, const EntityType* e, const std::vector<ImVec2>& points) const
{
	if (!enabled(e))
		return;

	if (outlined(e))
		drawList->AddPolyline(points.data(), (int)points.size(), outlineColor(e), ImDrawFlags_None, outlineThickness(e));

	drawList->AddPolyline(points.data(), (int)points.size(), color(e), ImDrawFlags_None, thickness(e));
}

void Line::renderGui()
{
	ImGui::PushID(id.c_str());
	for (Renderable* r : std::initializer_list<Renderable*>{
			 &enabled, &color, &thickness, &outlined,
			 &outlineColor, &outlineThickness })
		r->renderGui();
	ImGui::PopID();
}

SerializedTypeMap Line::serialize() const
{
	SerializedTypeMap map;
	for (const MixableBase* mixable : std::initializer_list<const MixableBase*>{
			 &enabled, &color, &thickness, &outlined,
			 &outlineColor, &outlineThickness })
		mixable->serialize(map);
	return map;
}

void Line::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &enabled, &color, &thickness, &outlined,
			 &outlineColor, &outlineThickness })
		mixable->deserializeFromParent(map);
}
