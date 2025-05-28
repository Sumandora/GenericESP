#ifndef GENERICESP_ELEMENT_TEXT_HPP
#define GENERICESP_ELEMENT_TEXT_HPP

#include "Element.hpp"

#include "imgui.h"

#include <optional>
#include <string>

namespace GenericESP {
	enum class TextAlignment {
		LEFT_BOUNDED,
		CENTERED,
		RIGHT_BOUNDED
	};

	enum class VerticalAlignment {
		BELOW_POSITION,
		CENTERED,
		ABOVE_POSITION
	};

	struct Text {
		virtual ~Text() = default;

		std::optional<ImVec2> draw(ImDrawList* drawList, const EntityType* e, const std::string& text, const ImVec2& pos,
			TextAlignment horizontalAlignment, VerticalAlignment verticalAlignment) const;
		[[nodiscard]] float getLineHeight(const EntityType* e) const;

		GENERICESP_SETTING(float, font_scale);
		GENERICESP_SETTING(ImColor, font_color);
		GENERICESP_SETTING(bool, shadow);
		GENERICESP_SETTING(float, shadow_offset);
		GENERICESP_SETTING(ImColor, shadow_color);
	};

}

#endif
