#ifndef GENERICESP_ELEMENT_SIDEDELEMENT_HPP
#define GENERICESP_ELEMENT_SIDEDELEMENT_HPP

#include "../Enum/Side.hpp"
#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {

	struct SidedElement : Element {
		MixableConfigurableValue<Side> side;

	private:
		// The renderer which is later replaced by the list renderer
		std::function<void(const std::string &,size_t &)> comboRenderer;
	public:

		explicit SidedElement(ESP* base, Side defaultSide);

		[[nodiscard]] const ImRect& chooseRect(const void* e, const UnionedRect& unionedRect) const;
		[[nodiscard]] ImRect& chooseRect(const void* e, UnionedRect& unionedRect) const;
	};

}

#endif
