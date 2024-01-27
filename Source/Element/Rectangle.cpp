#define IMGUI_DEFINE_MATH_OPERATORS

#include "GenericESP/Element/Rectangle.hpp"

#include "GenericESP/Serialization/ImColorSerialization.hpp"

using namespace GenericESP;

Rectangle::Rectangle(ESP* base, std::string id, bool topLevel)
	: Element(base, std::move(id), topLevel)
	, color{ StaticConfig<ImColor>{ "Color", { 1.0f, 1.0f, 1.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor } }
	, rounding{ StaticConfig<float>{ "Rounding", 0.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, thickness{ StaticConfig<float>{ "Thickness", 1.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, outlined{ StaticConfig<bool>{ "Outlined", true, base->createBoolRenderer() } }
	, outlineColor{ StaticConfig<ImColor>{ "Outline color", { 0.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor }, [this] {
					   const ConfigurableValue<bool>& selected = outlined.getSelected();
					   return !selected.isStatic() || selected.getStaticConfig().thing;
				   } }
	, outlineThickness{ StaticConfig<float>{ "Outline thickness", 2.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") }, [this] {
						   const ConfigurableValue<bool>& selected = outlined.getSelected();
						   return !selected.isStatic() || selected.getStaticConfig().thing;
					   } }
	, fill{ StaticConfig<bool>{ "Fill", false, base->createBoolRenderer() } }
	, fillColor{ StaticConfig<ImColor>{ "Fill color", { 1.0f, 1.0f, 1.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor }, [this] {
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
		drawList->AddRect(min, max, outlineColor(e), rounding, ImDrawFlags_None,
			outlineThickness);

	if (fill(e)) {
		const float halfThickness = thickness / 2.0f;
		const ImVec2 rectWidth{ halfThickness, halfThickness };
		drawList->AddRectFilled(min + rectWidth, max - rectWidth, fillColor(e), rounding, ImDrawFlags_None);
	}

	drawList->AddRect(min, max, color(e), rounding, ImDrawFlags_None, thickness);

	rect.expand(halfWidth);
}

void Rectangle::renderGui()
{
	ImGui::PushID(id.c_str());
	for (Renderable* r : std::initializer_list<Renderable*>{
			 &enabled, &color, &rounding, &thickness,
			 &outlined, &outlineColor, &outlineThickness, &fill,
			 &fillColor })
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
	return map;
}

void Rectangle::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &enabled, &color, &rounding, &thickness,
			 &outlined, &outlineColor, &outlineThickness, &fill,
			 &fillColor })
		mixable->deserializeFromParent(map);
}
