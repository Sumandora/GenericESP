#ifndef GENERICESP_ELEMENT_RECTANGLE_HPP
#define GENERICESP_ELEMENT_RECTANGLE_HPP

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {

	struct Rectangle : Element {
		MixableConfigurableValue<ImColor> color;
		MixableConfigurableValue<float> rounding;
		MixableConfigurableValue<float> thickness;
		MixableConfigurableValue<bool> outlined;
		MixableConfigurableValue<ImColor> outlineColor;
		MixableConfigurableValue<float> outlineThickness;
		MixableConfigurableValue<bool> fill;
		MixableConfigurableValue<ImColor> fillColor;

		explicit Rectangle(ESP* base);

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& rect) const;
		void renderGui(const std::string& id);
	};

}

#endif
