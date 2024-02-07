#define IMGUI_DEFINE_MATH_OPERATORS

#include "GenericESP/Element/Rectangle.hpp"

#include "GenericESP/RendererFactory.hpp"
#include "GenericESP/Serialization/ImColorSerialization.hpp"

using namespace GenericESP;

Rectangle::RoundedEdges::RoundedEdges(ESP* base, std::string id, bool topLevel)
	: Element(base, std::move(id), topLevel)
	, topLeft{ StaticConfig<bool>{ "Top left", true, rendererFactory.createBoolRenderer() } }
	, topRight{ StaticConfig<bool>{ "Top right", true, rendererFactory.createBoolRenderer() } }
	, bottomLeft{ StaticConfig<bool>{ "Bottom left", true, rendererFactory.createBoolRenderer() } }
	, bottomRight{ StaticConfig<bool>{ "Bottom right", true, rendererFactory.createBoolRenderer() } }
{
}

ImDrawFlags Rectangle::RoundedEdges::getRoundingFlags(const EntityType* e) const
{
	int flags = ImDrawFlags_RoundCornersNone;

	if (topLeft(e))
		flags |= ImDrawFlags_RoundCornersTopLeft;

	if (topRight(e))
		flags |= ImDrawFlags_RoundCornersTopRight;

	if (bottomLeft(e))
		flags |= ImDrawFlags_RoundCornersBottomLeft;

	if (bottomRight(e))
		flags |= ImDrawFlags_RoundCornersBottomRight;

	return flags;
}

void Rectangle::RoundedEdges::renderGui()
{
	ImGui::PushID(id.c_str());
	ImGui::Text("%s", id.c_str());
	ImGui::SameLine();
	if (ImGui::Button("..."))
		ImGui::OpenPopup("##Popup");

	if (ImGui::BeginPopup("##Popup")) {
		topLeft.renderGui(); ImGui::SameLine(); topRight.renderGui();
		bottomLeft.renderGui(); ImGui::SameLine(); bottomRight.renderGui();
		ImGui::EndPopup();
	}
	ImGui::PopID();
}

SerializedTypeMap Rectangle::RoundedEdges::serialize() const
{
	SerializedTypeMap map;
	for (const MixableBase* mixable : std::initializer_list<const MixableBase*>{
			 &topLeft, &topRight, &bottomLeft, &bottomRight })
		mixable->serialize(map);
	return map;
}

void Rectangle::RoundedEdges::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &topLeft, &topRight, &bottomLeft, &bottomRight })
		mixable->deserializeFromParent(map);
}

Rectangle::Rectangle(ESP* base, std::string id, bool topLevel)
	: Element(base, std::move(id), topLevel)
	, color{ StaticConfig<ImColor>{ "Color", { 1.0f, 1.0f, 1.0f, 1.0f }, rendererFactory.createColorRenderer(), serializeImColor, deserializeImColor } }
	, rounding{ StaticConfig<float>{ "Rounding", 0.0f, rendererFactory.createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, roundedEdges{ base, "Rounded edges", false }
	, thickness{ StaticConfig<float>{ "Thickness", 1.0f, rendererFactory.createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, outlined{ StaticConfig<bool>{ "Outlined", true, rendererFactory.createBoolRenderer() } }
	, outlineColor{ StaticConfig<ImColor>{ "Outline color", { 0.0f, 0.0f, 0.0f, 1.0f }, rendererFactory.createColorRenderer(), serializeImColor, deserializeImColor }, [this] {
					   const ConfigurableValue<bool>& selected = outlined.getSelected();
					   return !selected.isStatic() || selected.getStaticConfig().thing;
				   } }
	, outlineThickness{ StaticConfig<float>{ "Outline thickness", 2.0f, rendererFactory.createFloatRenderer(0.0f, 10.0f, "%.2f") }, [this] {
						   const ConfigurableValue<bool>& selected = outlined.getSelected();
						   return !selected.isStatic() || selected.getStaticConfig().thing;
					   } }
	, fill{ StaticConfig<bool>{ "Fill", false, rendererFactory.createBoolRenderer() } }
	, fillColor{ StaticConfig<ImColor>{ "Fill color", { 1.0f, 1.0f, 1.0f, 1.0f }, rendererFactory.createColorRenderer(), serializeImColor, deserializeImColor }, [this] {
					const ConfigurableValue<bool>& selected = fill.getSelected();
					return !selected.isStatic() || selected.getStaticConfig().thing;
				} }
{
}

void Rectangle::draw(ImDrawList* drawList, const EntityType* e, UnionedRect& rect) const
{
	if (!enabled(e))
		return;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float rounding = this->rounding(e);

	const float thickness = this->thickness(e);
	const bool outlined = this->outlined(e);
	const float outlineThickness = this->outlineThickness(e);
#pragma clang diagnostic pop

	const float totalWidth = outlined ? std::max(thickness, outlineThickness) : thickness;
	const float halfWidth = totalWidth / 2;

	const ImVec2 min = rect.getMin();
	const ImVec2 max = rect.getMax();

	if (outlined)
		drawList->AddRect(min, max, outlineColor(e), rounding, roundedEdges.getRoundingFlags(e), outlineThickness);

	if (fill(e)) {
		const float halfThickness = thickness / 2.0f;
		const ImVec2 rectWidth{ halfThickness, halfThickness };
		drawList->AddRectFilled(min + rectWidth, max - rectWidth, fillColor(e), rounding, roundedEdges.getRoundingFlags(e));
	}

	drawList->AddRect(min, max, color(e), rounding, roundedEdges.getRoundingFlags(e), thickness);

	rect.expand(halfWidth);
}

void Rectangle::renderGui()
{
	ImGui::PushID(id.c_str());
	for (Renderable* r : std::initializer_list<Renderable*>{
			 &enabled, &color, &rounding, &roundedEdges,
			 &thickness, &outlined, &outlineColor, &outlineThickness,
			 &fill, &fillColor })
		r->renderGui();
	ImGui::PopID();
}

SerializedTypeMap Rectangle::serialize() const
{
	SerializedTypeMap map;
	for (const MixableBase* mixable : std::initializer_list<const MixableBase*>{
			 &enabled, &color, &rounding, &thickness,
			 &outlined, &outlineColor, &outlineThickness, &fill,
			 &fillColor })
		mixable->serialize(map);

	map.putSubtree("Rounded edges", roundedEdges.serialize());
	return map;
}

void Rectangle::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &enabled, &color, &rounding, &thickness,
			 &outlined, &outlineColor, &outlineThickness, &fill,
			 &fillColor })
		mixable->deserializeFromParent(map);
	auto opt = map.getSubtree("Rounded edges");
	if(opt.has_value())
		roundedEdges.deserialize(opt.value());
}
