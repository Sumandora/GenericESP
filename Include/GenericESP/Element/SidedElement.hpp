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

	inline std::initializer_list<std::string> sideLocalization{
		"Top",
		"Left",
		"Bottom",
		"Right"
	};

	struct SidedElement : Element {
		MixableConfigurableValue<Side> side;

		// The renderer which is later replaced by the list renderer
		std::function<void(const std::string &,size_t &)> comboRenderer;

		explicit SidedElement(ESP* base, Side defaultSide);

		[[nodiscard]] const ImRect& chooseRect(const EntityType* e, const UnionedRect& unionedRect) const;
		[[nodiscard]] ImRect& chooseRect(const EntityType* e, UnionedRect& unionedRect) const;
	};

}

#endif
