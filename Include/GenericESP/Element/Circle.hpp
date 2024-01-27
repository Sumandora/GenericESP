#ifndef GENERICESP_ELEMENT_CIRCLE_HPP
#define GENERICESP_ELEMENT_CIRCLE_HPP

#include "Element.hpp"

namespace GenericESP {

	struct Circle : Element {
		Mixable<ImColor> color;
		Mixable<float> radius;
		Mixable<bool> outlined;
		Mixable<ImColor> outlineColor;
		Mixable<float> outlineRadius;

		explicit Circle(ESP* base, std::string id, bool topLevel = true);

		void draw(ImDrawList* drawList, const EntityType* e, const ImVec2& position) const;
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap& map) override;
	};

}

#endif
