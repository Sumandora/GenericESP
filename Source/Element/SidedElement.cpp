#include "GenericESP/Element/SidedElement.hpp"
#include "GenericESP/Element/Element.hpp"
#include "GenericESP/UnionedRect.hpp"
#include "imgui_internal.h"

#include <stdexcept>

using namespace GenericESP;

const ImRect& SidedElement::choose_rect(const EntityType* e, const UnionedRect& unioned_rect) const
{
	switch (get_side(e)) {
	case Side::LEFT:
	case Side::RIGHT:
		return unioned_rect.horizontal;
	case Side::TOP:
	case Side::BOTTOM:
		return unioned_rect.vertical;
	default:
		throw std::runtime_error("Side was invalid");
	}
}

ImRect& SidedElement::choose_rect(const EntityType* e, UnionedRect& unioned_rect) const
{
	switch (get_side(e)) {
	case Side::LEFT:
	case Side::RIGHT:
		return unioned_rect.horizontal;
	case Side::TOP:
	case Side::BOTTOM:
		return unioned_rect.vertical;
	default:
		throw std::runtime_error("Side was invalid");
	}
}
