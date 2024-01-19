#ifndef GENERICESP_ELEMENT_CIRCLE_HPP
#define GENERICESP_ELEMENT_CIRCLE_HPP

#include "Element.hpp"

namespace GenericESP {

	struct Circle : Element {
		MixableConfigurableValue<ImColor> circleColor;
		MixableConfigurableValue<float> radius;
		MixableConfigurableValue<bool> outlined;
		MixableConfigurableValue<ImColor> outlineColor;
		MixableConfigurableValue<float> outlineRadius;

		explicit Circle(ESP* base, std::string&& id);

		void draw(ImDrawList* drawList, const EntityType* e, const ImVec2& position) const;
		void renderGui();
	};

}

#endif
