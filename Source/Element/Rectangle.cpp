#define IMGUI_DEFINE_MATH_OPERATORS

#include "GenericESP/Element/Rectangle.hpp"

using namespace GenericESP;

Rectangle::Rectangle(GenericESP::ESP* base)
	: Element(base)
	, color{ "Color", StaticConfig<ImColor>{ { 1.0f, 1.0f, 1.0f, 1.0f }, base->createColorRenderer() } }
	, rounding{ "Rounding", StaticConfig<float>{ 0.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, thickness{ "Thickness", StaticConfig<float>{ 1.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, outlined{ "Outlined", StaticConfig<bool>{ true, base->createBoolRenderer() } }
	, outlineColor{ "Outline color", StaticConfig<ImColor>{ { 0.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer() }, [this] {
					   const ConfigurableValue<bool>& selected = outlined.getSelected();
					   return !selected.isStatic() || selected.getStaticConfig().thing;
				   } }
	, outlineThickness{ "Outline thickness", StaticConfig<float>{ 2.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") }, [this] {
						   const ConfigurableValue<bool>& selected = outlined.getSelected();
						   return !selected.isStatic() || selected.getStaticConfig().thing;
					   } }
	, fill{ "Fill", StaticConfig<bool>{ false, base->createBoolRenderer() } }
	, fillColor{ "Fill color", StaticConfig<ImColor>{ { 1.0f, 1.0f, 1.0f, 1.0f }, base->createColorRenderer() }, [this] {
					const ConfigurableValue<bool>& selected = fill.getSelected();
					return !selected.isStatic() || selected.getStaticConfig().thing;
				} }
{
}

void Rectangle::draw(ImDrawList* drawList, const EntityType* e, GenericESP::UnionedRect& rect) const
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

void Rectangle::renderGui(const std::string& id)
{
	ImGui::PushID(id.c_str());
	enabled.renderGui();
	color.renderGui();
	rounding.renderGui();
	thickness.renderGui();
	outlined.renderGui();
	outlineColor.renderGui();
	outlineThickness.renderGui();
	fill.renderGui();
	fillColor.renderGui();
	ImGui::PopID();
}
