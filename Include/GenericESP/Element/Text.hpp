#ifndef GENERICESP_ELEMENT_TEXT_HPP
#define GENERICESP_ELEMENT_TEXT_HPP

#include "Element.hpp"

#include "imgui.h"

#include <cstdint>
#include <optional>
#include <string>

namespace GenericESP {
	enum class TextAlignment : std::uint8_t {
		LEFT_BOUNDED,
		CENTERED,
		RIGHT_BOUNDED
	};

	enum class VerticalAlignment : std::uint8_t {
		BELOW_POSITION,
		CENTERED,
		ABOVE_POSITION
	};

	struct Text {
		GENERICESP_SETTING(ImFont*, font);
		GENERICESP_SETTING(float, font_size);
		GENERICESP_SETTING(ImColor, font_color);
		GENERICESP_SETTING(bool, shadow);
		GENERICESP_SETTING(float, shadow_offset);
		GENERICESP_SETTING(ImColor, shadow_color);

		virtual ~Text() = default;

		std::optional<ImVec2> draw(ImDrawList* draw_list, const EntityType* e, const std::string& text, const ImVec2& pos,
			TextAlignment horizontal_alignment, VerticalAlignment vertical_alignment) const;
		[[nodiscard]] ImVec2 get_text_size(const std::string& text, const EntityType* e) const;
		[[nodiscard]] static ImVec2 get_text_size(const std::string& text, ImFont* font, float font_size);
	};

}

#endif
