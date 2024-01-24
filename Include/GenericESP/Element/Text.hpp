#ifndef GENERICESP_ELEMENT_TEXT_HPP
#define GENERICESP_ELEMENT_TEXT_HPP

#include "Element.hpp"

#include <optional>

namespace GenericESP {
	enum class TextAlignment {
		LEFT_BOUNDED,
		CENTERED,
		RIGHT_BOUNDED
	};

	enum class VerticalAlignment {
		BELOW_POSITION,
		CENTERED,
		ABOVE_POSITION
	};

	struct Text : Element {
		Mixable<float> fontScale;
		Mixable<ImColor> fontColor;
		Mixable<bool> shadow;
		Mixable<float> shadowOffset;
		Mixable<ImColor> shadowColor;

		explicit Text(ESP* base, std::string id);

		std::optional<ImVec2> draw(ImDrawList* drawList, const EntityType* e, const std::string& text, const ImVec2& pos,
			TextAlignment horizontalAlignment, VerticalAlignment verticalAlignment) const;
		[[nodiscard]] float getLineHeight(const EntityType* e) const;
		void renderGui();

		SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap& map) override;
	};

}

#endif
