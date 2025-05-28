#ifndef GENERICESP_UNIONEDRECT_HPP
#define GENERICESP_UNIONEDRECT_HPP

#include "imgui.h"
#include "imgui_internal.h"

namespace GenericESP {

	struct UnionedRect {
		ImRect horizontal, vertical;

		explicit UnionedRect(ImRect rect);

		[[nodiscard]] ImVec2 get_min() const;
		[[nodiscard]] ImVec2 get_max() const;

		void expand_horizontally(float amount);
		void expand_vertically(float amount);
		void expand(float amount);
	};

}

#endif
