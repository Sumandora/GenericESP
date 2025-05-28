#ifndef GENERICESP_ELEMENT_SIDEDTEXT_HPP
#define GENERICESP_ELEMENT_SIDEDTEXT_HPP

#include "SidedElement.hpp"
#include "Text.hpp"

namespace GenericESP {

	struct SidedText : SidedElement, Text {
		GENERICESP_SETTING(float, spacing);

		void draw(ImDrawList* drawList, const EntityType* e, const std::string& text, UnionedRect& unionedRect) const;

		float get_font_scale(const EntityType*) const override = 0;
		ImColor get_font_color(const EntityType*) const override = 0;
		bool get_shadow(const EntityType*) const override = 0;
		float get_shadow_offset(const EntityType*) const override = 0;
		ImColor get_shadow_color(const EntityType*) const override = 0;
	};

}

#endif
