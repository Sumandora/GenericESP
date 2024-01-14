#include "GenericESP/UnionedRect.hpp"

using namespace GenericESP;

UnionedRect::UnionedRect(const ImRect rect)
	: horizontal(rect)
	, vertical(rect)
{

}

ImVec2 UnionedRect::getMin() const
{
	return { std::min(horizontal.Min.x, vertical.Min.x), std::min(horizontal.Min.y, vertical.Min.y) };
}

ImVec2 UnionedRect::getMax() const
{
	return { std::min(horizontal.Max.x, vertical.Max.x), std::min(horizontal.Max.y, vertical.Max.y) };
}

void UnionedRect::expandHorizontally(const float amount)
{
	horizontal.Min.x -= amount;
	horizontal.Max.x += amount;

	horizontal.Min.y -= amount;
	horizontal.Max.y += amount;
}

void UnionedRect::expandVertically(const float amount)
{
	vertical.Min.x -= amount;
	vertical.Max.x += amount;

	vertical.Min.y -= amount;
	vertical.Max.y += amount;
}

void UnionedRect::expand(const float amount)
{
	expandHorizontally(amount);
	expandVertically(amount);
}