#include "GenericESP/Element/SidedElement.hpp"

using namespace GenericESP;

SidedElement::SidedElement(ESP* base, GenericESP::Side defaultSide)
	: Element(base)
	, comboRenderer(base->createComboRenderer(sideLocalization))
	, side{
		"Side",
		StaticConfig<Side>{ static_cast<Side>(-1 /*placeholder*/), [&](const std::string& id, Side& side) {
							   auto idx = static_cast<std::size_t>(side);
							   comboRenderer(id, idx);
							   side = static_cast<Side>(idx);
						   } }
	}
{
	side.getSelected().getStaticConfig().thing = defaultSide;
}

const ImRect& SidedElement::chooseRect(const void* e, const UnionedRect& unionedRect) const
{
	switch (side(e)) {
	case Side::LEFT:
	case Side::RIGHT:
		return unionedRect.horizontal;
	case Side::TOP:
	case Side::BOTTOM:
		return unionedRect.vertical;
	default:
		throw std::runtime_error("Side was invalid");
	}
}

ImRect& SidedElement::chooseRect(const void* e, UnionedRect& unionedRect) const
{
	switch (side(e)) {
	case Side::LEFT:
	case Side::RIGHT:
		return unionedRect.horizontal;
	case Side::TOP:
	case Side::BOTTOM:
		return unionedRect.vertical;
	default:
		throw std::runtime_error("Side was invalid");
	}
}
