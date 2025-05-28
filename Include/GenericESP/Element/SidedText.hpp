#ifndef GENERICESP_ELEMENT_SIDEDTEXT_HPP
#define GENERICESP_ELEMENT_SIDEDTEXT_HPP

#include "../UnionedRect.hpp"
#include "Element.hpp"
#include "SidedElement.hpp"
#include "Text.hpp"

#include "imgui.h"

#include <string>

namespace GenericESP {

	struct SidedText : SidedElement, Text {
		GENERICESP_SETTING(float, spacing);

		void draw(ImDrawList* draw_list, const EntityType* e, const std::string& text, UnionedRect& unioned_rect) const;

		float get_font_scale(const EntityType* e) const override = 0;
		ImColor get_font_color(const EntityType* e) const override = 0;
		bool get_shadow(const EntityType* e) const override = 0;
		float get_shadow_offset(const EntityType* e) const override = 0;
		ImColor get_shadow_color(const EntityType* e) const override = 0;
	};

}

#endif
