#ifndef GENERICESP_ELEMENT_RECTANGLE_HPP
#define GENERICESP_ELEMENT_RECTANGLE_HPP

#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {

	struct Rectangle : Element {
		Mixable<ImColor> color;
		Mixable<float> rounding;
		Mixable<float> thickness;
		Mixable<bool> outlined;
		Mixable<ImColor> outlineColor;
		Mixable<float> outlineThickness;
		Mixable<bool> fill;
		Mixable<ImColor> fillColor;

		explicit Rectangle(ESP* base, std::string id);

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& rect) const;
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap &map) override;
	};

}

#endif
