#include "GenericESP/Element/SidedElement.hpp"

using namespace GenericESP;

SidedElement::SidedElement(ESP* base, std::string&& id, GenericESP::Side defaultSide)
	: Element(base, std::move(id))
	, side{
		"Side",
		StaticConfig<std::size_t>{ 0 /* placeholder */, base->createComboRenderer(sideLocalization) }
	}
{
	side.getSelected().getStaticConfig().thing = static_cast<std::size_t>(defaultSide);
}

Side SidedElement::getSide(const EntityType* e) const {
	return static_cast<Side>(side(e));
}

const ImRect& SidedElement::chooseRect(const EntityType* e, const UnionedRect& unionedRect) const
{
	switch (getSide(e)) {
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

ImRect& SidedElement::chooseRect(const EntityType* e, UnionedRect& unionedRect) const
{
	switch (getSide(e)) {
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
