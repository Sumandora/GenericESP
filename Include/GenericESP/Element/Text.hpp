#ifndef GENERICESP_ELEMENT_TEXT_HPP
#define GENERICESP_ELEMENT_TEXT_HPP

#include "../Element/Element.hpp"
#include "../Enum/Text.hpp"

#include <optional>

namespace GenericESP {

	struct Text : Element {
		MixableConfigurableValue<float> fontScale;
		MixableConfigurableValue<ImColor> fontColor;
		MixableConfigurableValue<bool> shadow;
		MixableConfigurableValue<float> shadowOffset;
		MixableConfigurableValue<ImColor> shadowColor;

		explicit Text(ESP* base);

		std::optional<ImVec2> draw(ImDrawList* drawList, const EntityType* e, const std::string& text, const ImVec2& pos, TextAlignment horizontalAlignment, VerticalAlignment verticalAlignment) const;
		[[nodiscard]] float getLineHeight(const EntityType* e) const;
		void renderGui(const std::string& id);
	};

}

#endif
