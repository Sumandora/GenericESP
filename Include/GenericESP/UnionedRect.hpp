#ifndef GENERICESP_UNIONEDRECT_HPP
#define GENERICESP_UNIONEDRECT_HPP

#include "imgui.h"
#include "imgui_internal.h"

namespace GenericESP {

	struct UnionedRect {
		ImRect horizontal, vertical;

		explicit UnionedRect(ImRect rect);

		[[nodiscard]] ImVec2 getMin() const;
		[[nodiscard]] ImVec2 getMax() const;

		void expandHorizontally(float amount);
		void expandVertically(float amount);
		void expand(float amount);
	};

}

#endif
