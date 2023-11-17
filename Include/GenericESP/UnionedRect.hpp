#ifndef GENERICESP_UNIONEDRECT_HPP
#define GENERICESP_UNIONEDRECT_HPP

#include "imgui.h"
#include "imgui_internal.h"

namespace GenericESP {

	struct UnionedRect {
		ImRect horizontal, vertical;

		explicit UnionedRect(const ImRect rect)
			: horizontal(rect)
			, vertical(rect)
		{
		}

		[[nodiscard]] ImVec2 getMin() const
		{
			return { std::min(horizontal.Min.x, vertical.Min.x), std::min(horizontal.Min.y, vertical.Min.y) };
		}

		[[nodiscard]] ImVec2 getMax() const
		{
			return { std::min(horizontal.Max.x, vertical.Max.x), std::min(horizontal.Max.y, vertical.Max.y) };
		}

		void expand(const float amount)
		{
			horizontal.Min.x -= amount;
			horizontal.Max.x += amount;

			horizontal.Min.y -= amount;
			horizontal.Max.y += amount;

			vertical.Min.x -= amount;
			vertical.Max.x += amount;

			vertical.Min.y -= amount;
			vertical.Max.y += amount;
		}
	};

}

#endif
