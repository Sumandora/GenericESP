#include "GenericESP/Element/SidedElement.hpp"

#include "GenericESP/RendererFactory.hpp"

using namespace GenericESP;

std::initializer_list<std::string> GenericESP::sideLocalization{
	"Top",
	"Left",
	"Bottom",
	"Right"
};

SidedElement::SidedElement(ESP* base, std::string id, Side defaultSide, bool topLevel)
	: Element(base, std::move(id), topLevel)
	, side{ StaticConfig<std::size_t>{ "Side", static_cast<std::size_t>(defaultSide), rendererFactory->createComboRenderer(sideLocalization) } }
{
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
