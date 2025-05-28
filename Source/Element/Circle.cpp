#include "GenericESP/Element/Circle.hpp"

using namespace GenericESP;

void Circle::draw(ImDrawList* drawList, const EntityType* e, const ImVec2& position) const
{
	if (get_outlined(e))
		drawList->AddCircleFilled(position, get_outline_radius(e), get_outline_color(e));

	drawList->AddCircleFilled(position, get_radius(e), get_color(e));
}
