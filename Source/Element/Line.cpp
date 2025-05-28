#include "GenericESP/Element/Line.hpp"

#include "GenericESP/Element/Element.hpp"

#include "imgui.h"

#include <vector>

using namespace GenericESP;

void Line::draw(ImDrawList* draw_list, const EntityType* e, const std::vector<ImVec2>& points) const
{
	if (get_outlined(e))
		draw_list->AddPolyline(points.data(), static_cast<int>(points.size()), get_outline_color(e), ImDrawFlags_None, get_outline_thickness(e));

	draw_list->AddPolyline(points.data(), static_cast<int>(points.size()), get_color(e), ImDrawFlags_None, get_thickness(e));
}
