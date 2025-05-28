#ifndef GENERICESP_ELEMENT_FLAGS_HPP
#define GENERICESP_ELEMENT_FLAGS_HPP

#include "../Flag.hpp"
#include "GenericESP/Element/Element.hpp"
#include "SidedElement.hpp"

#include <memory>
#include <ranges>
#include <span>

namespace GenericESP {

	struct Flags : SidedElement {
		GENERICESP_SETTING(float, spacing);
		GENERICESP_SETTING(float, line_spacing);

		GENERICESP_SETTING(const std::vector<Flag*>&, order);

		virtual ~Flags() = default;

		// --- Likely irrelevant for users ---
		ImVec2 drawEntry(ImDrawList* drawList, const EntityType* e, ImRect& rect, const Flag& flag, float yOffset) const;
		// --- Likely irrelevant for users ---

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const;
	};

}

#endif
