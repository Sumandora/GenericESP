#include "GenericESP/UnionedRect.hpp"

#include <algorithm>

#include "imgui.h"
#include "imgui_internal.h"

using namespace GenericESP;

UnionedRect::UnionedRect(const ImRect rect)
	: horizontal(rect)
	, vertical(rect)
{

}

ImVec2 UnionedRect::get_min() const
{
	return { std::min(horizontal.Min.x, vertical.Min.x), std::min(horizontal.Min.y, vertical.Min.y) };
}

ImVec2 UnionedRect::get_max() const
{
	return { std::min(horizontal.Max.x, vertical.Max.x), std::min(horizontal.Max.y, vertical.Max.y) };
}

void UnionedRect::expand_horizontally(const float amount)
{
	horizontal.Min.x -= amount;
	horizontal.Max.x += amount;

	horizontal.Min.y -= amount;
	horizontal.Max.y += amount;
}

void UnionedRect::expand_vertically(const float amount)
{
	vertical.Min.x -= amount;
	vertical.Max.x += amount;

	vertical.Min.y -= amount;
	vertical.Max.y += amount;
}

void UnionedRect::expand(const float amount)
{
	expand_horizontally(amount);
	expand_vertically(amount);
}
