#ifndef GENERICESP_ELEMENT_LINE_HPP
#define GENERICESP_ELEMENT_LINE_HPP

#include "Element.hpp"

namespace GenericESP {

	struct Line : Element {
		Mixable<ImColor> color;
		Mixable<float> thickness;
		Mixable<bool> outlined;
		Mixable<ImColor> outlineColor;
		Mixable<float> outlineThickness;

		explicit Line(ESP* base, std::string id, bool topLevel = true);

		void draw(ImDrawList* drawList, const EntityType* e, const std::vector<ImVec2>& points) const;
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap &map) override;
	};

}

#endif
