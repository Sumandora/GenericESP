#ifndef GENERICESP_ELEMENT_LINE_HPP
#define GENERICESP_ELEMENT_LINE_HPP

#include "Element.hpp"

#include "imgui.h"

#include <vector>

namespace GenericESP {

	struct Line {
		virtual ~Line() = default;

		void draw(ImDrawList* drawList, const EntityType* e, const std::vector<ImVec2>& points) const;

		GENERICESP_SETTING(ImColor, color);
		GENERICESP_SETTING(float, thickness);
		GENERICESP_SETTING(bool, outlined);
		GENERICESP_SETTING(ImColor, outline_color);
		GENERICESP_SETTING(float, outline_thickness);
	};

}

#endif
