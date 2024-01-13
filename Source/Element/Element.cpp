#include "GenericESP/Element/Element.hpp"

#include "GenericESP/Config/MixableConfigurationValue.hpp"

GenericESP::Element::Element(ESP* base)
	: enabled("Enabled", GenericESP::StaticConfig<bool>{ false, base->createBoolRenderer() })
{
}