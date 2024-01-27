#include "GenericESP/Element/Element.hpp"

#include <algorithm>

GenericESP::Element::Element(ESP* base, std::string id, bool topLevel)
	: base(base)
	, id(std::move(id))
	, topLevel(topLevel)
	, enabled(StaticConfig<bool>{ "Enabled", false, base->createBoolRenderer() })
{
	std::cout << this->id.c_str() << " " << topLevel << std::endl;
	if(topLevel)
		base->elements.emplace_back(this);
}

GenericESP::Element::~Element()
{
	if(topLevel)
		std::ranges::remove_if(base->elements, [this](const std::unique_ptr<Element>& e) { return e.get() == this; });
}