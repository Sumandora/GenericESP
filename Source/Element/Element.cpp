#include "GenericESP/Element/Element.hpp"

#include "GenericESP/Config/MixableConfigurationValue.hpp"

GenericESP::Element::Element(ESP* base, std::string&& id)
	: id(std::move(id))
	, enabled("Enabled", GenericESP::StaticConfig<bool>{ false, base->createBoolRenderer() })
{
}