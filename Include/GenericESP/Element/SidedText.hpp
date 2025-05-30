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
	};

}

#endif
