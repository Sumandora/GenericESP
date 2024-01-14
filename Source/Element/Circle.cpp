#include "GenericESP/Element/Circle.hpp"

using namespace GenericESP;

Circle::Circle(GenericESP::ESP* base)
	: Element(base)
	, circleColor{ "Circle color", StaticConfig<ImColor>{ { 1.0f, 1.0f, 1.0f, 1.0f }, base->createColorRenderer() } }
	, radius{ "Radius", StaticConfig<float>{ 1.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, outlined{ "Outlined", StaticConfig<bool>{ true, base->createBoolRenderer() } }
	, outlineColor{ "Outline color", StaticConfig<ImColor>{ { 0.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer() } }
	, outlineRadius{ "Outline radius", StaticConfig<float>{ 2.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
{
}

void Circle::draw(ImDrawList* drawList, const EntityType* e, const ImVec2& position) const
{
	if (!enabled(e))
		return;

	if (outlined(e))
		drawList->AddCircleFilled(position, outlineRadius(e), outlineColor(e));

	drawList->AddCircleFilled(position, radius(e), circleColor(e));
}

void Circle::renderGui(const std::string& id)
{
	ImGui::PushID(id.c_str());
	enabled.renderGui();
	circleColor.renderGui();
	radius.renderGui();
	outlined.renderGui();
	outlineColor.renderGui();
	outlineRadius.renderGui();
	ImGui::PopID();
}
