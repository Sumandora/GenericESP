#include "GenericESP/Element/Circle.hpp"

#include "GenericESP/Element/Element.hpp"

#include "imgui.h"

using namespace GenericESP;

void Circle::draw(ImDrawList* draw_list, const EntityType* e, const ImVec2& position) const
{
	if (get_outlined(e))
		draw_list->AddCircleFilled(position, get_outline_radius(e), get_outline_color(e));

	draw_list->AddCircleFilled(position, get_radius(e), get_color(e));
}
