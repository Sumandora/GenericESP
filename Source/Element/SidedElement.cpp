#include "GenericESP/Element/SidedElement.hpp"

#include "GenericESP/RendererFactory.hpp"
#include <stdexcept>

using namespace GenericESP;

const ImRect& SidedElement::chooseRect(const EntityType* e, const UnionedRect& unionedRect) const
{
	switch (get_side(e)) {
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
	switch (get_side(e)) {
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
