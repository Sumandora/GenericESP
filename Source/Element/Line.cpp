#include "GenericESP/Element/Line.hpp"

using namespace GenericESP;

Line::Line(GenericESP::ESP* base)
	: Element(base)
	, lineColor{
		"Line color",
		StaticConfig<ImColor>{ { 1.0f, 1.0f, 1.0f, 1.0f }, base->createColorRenderer() }
	}
	, thickness{ "Thickness", StaticConfig<float>{ 1.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
	, outlined{ "Outlined", StaticConfig<bool>{ true, base->createBoolRenderer() } }
	, outlineColor{ "Outline color", StaticConfig<ImColor>{ { 0.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer() } }
	, outlineThickness{ "Outline thickness", StaticConfig<float>{ 2.0f, base->createFloatRenderer(0.0f, 10.0f, "%.2f") } }
{
}

void Line::draw(ImDrawList* drawList, const void* e, const std::vector<ImVec2>& points) const
{
	if (!enabled(e))
		return;

	if (outlined(e))
		drawList->AddPolyline(points.data(), (int)points.size(), outlineColor(e), ImDrawFlags_None, outlineThickness(e));

	drawList->AddPolyline(points.data(), (int)points.size(), lineColor(e), ImDrawFlags_None, thickness(e));
}

void Line::renderGui(const std::string& id)
{
	ImGui::PushID(id.c_str());
	enabled.renderGui();
	lineColor.renderGui();
	thickness.renderGui();
	outlined.renderGui();
	outlineColor.renderGui();
	outlineThickness.renderGui();
	ImGui::PopID();
}
