#ifndef GENERICESP_ELEMENT_SIDEDELEMENT_HPP
#define GENERICESP_ELEMENT_SIDEDELEMENT_HPP

#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {
	enum class Side {
		TOP,
		LEFT,
		BOTTOM,
		RIGHT
	};

	extern std::initializer_list<std::string> sideLocalization;

	struct SidedElement : Element {
		Mixable<std::size_t> side;

		// The renderer which is later replaced by the list renderer
		std::function<void(const std::string&, std::size_t&)> comboRenderer;

		explicit SidedElement(ESP* base, std::string id, Side defaultSide);

		Side getSide(const EntityType* e) const;

		[[nodiscard]] const ImRect& chooseRect(const EntityType* e, const UnionedRect& unionedRect) const;
		[[nodiscard]] ImRect& chooseRect(const EntityType* e, UnionedRect& unionedRect) const;
	};

}

#endif
