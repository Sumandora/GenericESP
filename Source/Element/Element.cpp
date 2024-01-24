#include "GenericESP/Element/Element.hpp"

GenericESP::Element::Element(ESP* base, std::string id)
	: id(std::move(id))
	, enabled(StaticConfig<bool>{ "Enabled", false, base->createBoolRenderer() })
{
}