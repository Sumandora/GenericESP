#include "GenericESP/Element/Element.hpp"

#include <algorithm>

GenericESP::Element::Element(ESP* base, std::string id)
	: base(base)
	, id(std::move(id))
	, enabled(StaticConfig<bool>{ "Enabled", false, base->createBoolRenderer() })
{
	base->elements.push_back(this);
}

GenericESP::Element::~Element()
{
	std::ranges::remove(base->elements, this);
}