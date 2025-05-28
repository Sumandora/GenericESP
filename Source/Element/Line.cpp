#include "GenericESP/Element/Line.hpp"

using namespace GenericESP;

void Line::draw(ImDrawList* drawList, const EntityType* e, const std::vector<ImVec2>& points) const
{
	if (get_outlined(e))
		drawList->AddPolyline(points.data(), (int)points.size(), get_outline_color(e), ImDrawFlags_None, get_outline_thickness(e));

	drawList->AddPolyline(points.data(), (int)points.size(), get_color(e), ImDrawFlags_None, get_thickness(e));
}
