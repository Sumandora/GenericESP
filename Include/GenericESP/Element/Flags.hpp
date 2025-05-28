#ifndef GENERICESP_ELEMENT_FLAGS_HPP
#define GENERICESP_ELEMENT_FLAGS_HPP

#include "../Flag.hpp"
#include "../UnionedRect.hpp"
#include "Element.hpp"
#include "SidedElement.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include <vector>

namespace GenericESP {

	struct Flags : SidedElement {
		GENERICESP_SETTING(float, spacing);
		GENERICESP_SETTING(float, line_spacing);

		GENERICESP_SETTING(const std::vector<Flag*>&, order);

		virtual ~Flags() = default;

		// --- Likely irrelevant for users ---
		ImVec2 draw_entry(ImDrawList* draw_list, const EntityType* e, ImRect& rect, const Flag& flag, float y_offset) const;
		// --- Likely irrelevant for users ---

		void draw(ImDrawList* draw_list, const EntityType* e, UnionedRect& unioned_rect) const;
	};

}

#endif
