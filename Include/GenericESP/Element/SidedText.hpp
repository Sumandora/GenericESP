#ifndef GENERICESP_ELEMENT_SIDEDTEXT_HPP
#define GENERICESP_ELEMENT_SIDEDTEXT_HPP

#include "SidedElement.hpp"
#include "Text.hpp"

namespace GenericESP {

	struct SidedText : SidedElement {
		using SidedElement::enabled;
		using SidedElement::side;
		MixableConfigurableValue<float> spacing;
		Text textElement;

		explicit SidedText(ESP* base);

		void draw(ImDrawList* drawList, const EntityType* e, const std::string& text, UnionedRect& unionedRect) const;
		void renderGui(const std::string& id);
	};

}

#endif
