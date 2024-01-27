#ifndef GENERICESP_ELEMENT_SIDEDTEXT_HPP
#define GENERICESP_ELEMENT_SIDEDTEXT_HPP

#include "SidedElement.hpp"
#include "Text.hpp"

namespace GenericESP {

	struct SidedText : SidedElement {
		Mixable<float> spacing;
		Text textElement;

		explicit SidedText(ESP* base, std::string id, Side side, bool topLevel = true);

		void draw(ImDrawList* drawList, const EntityType* e, const std::string& text, UnionedRect& unionedRect) const;
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap &map) override;
	};

}

#endif
