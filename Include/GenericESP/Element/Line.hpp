#ifndef GENERICESP_ELEMENT_LINE_HPP
#define GENERICESP_ELEMENT_LINE_HPP

#include "Element.hpp"

namespace GenericESP {

	struct Line : Element {
		MixableConfigurableValue<ImColor> lineColor;
		MixableConfigurableValue<float> thickness;
		MixableConfigurableValue<bool> outlined;
		MixableConfigurableValue<ImColor> outlineColor;
		MixableConfigurableValue<float> outlineThickness;

		explicit Line(ESP* base, std::string&& id);

		void draw(ImDrawList* drawList, const EntityType* e, const std::vector<ImVec2>& points) const;
		void renderGui();
	};

}

#endif
