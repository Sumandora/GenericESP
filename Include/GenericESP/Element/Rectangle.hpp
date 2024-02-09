#ifndef GENERICESP_ELEMENT_RECTANGLE_HPP
#define GENERICESP_ELEMENT_RECTANGLE_HPP

#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {

	struct Rectangle : Element {
		Mixable<ImColor> color;
		Mixable<float> rounding;

		struct RoundedEdges : Element {
			Mixable<bool> topLeft;
			Mixable<bool> topRight;
			Mixable<bool> bottomLeft;
			Mixable<bool> bottomRight;

			PopupRenderer popupRenderer;

			RoundedEdges(ESP* base, std::string id, bool topLevel = true);

			ImDrawFlags getRoundingFlags(const EntityType* e) const;

			void renderGui() override;
			[[nodiscard]] SerializedTypeMap serialize() const override;
			void deserialize(const SerializedTypeMap &map) override;
		} roundedEdges;

		Mixable<float> thickness;
		Mixable<bool> outlined;
		Mixable<ImColor> outlineColor;
		Mixable<float> outlineThickness;
		Mixable<bool> fill;
		Mixable<ImColor> fillColor;

		explicit Rectangle(ESP* base, std::string id, bool topLevel = true);

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& rect) const;
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap &map) override;
	};

}

#endif
