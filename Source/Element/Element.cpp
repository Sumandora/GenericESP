#include "GenericESP/Element/Element.hpp"

#include "GenericESP/RendererFactory.hpp"

#include <algorithm>

GenericESP::Element::Element(ESP* base, std::string id, bool topLevel)
	: base(base)
	, id(std::move(id))
	, topLevel(topLevel)
	, enabled(StaticConfig<bool>{ "Enabled", false, rendererFactory.createBoolRenderer() })
{
	if(topLevel)
		base->elements.emplace_back(this);
}

GenericESP::Element::~Element()
{
	if(topLevel)
		std::ranges::remove(base->elements, this);
}
