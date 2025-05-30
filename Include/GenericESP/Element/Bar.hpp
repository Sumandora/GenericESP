#ifndef GENERICESP_ELEMENT_BAR_HPP
#define GENERICESP_ELEMENT_BAR_HPP

#include "../OpaqueLambda.hpp"
#include "../UnionedRect.hpp"
#include "Element.hpp"
#include "SidedElement.hpp"
#include "Text.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include <array>
#include <optional>
#include <string>

namespace GenericESP {

	struct Bar : SidedElement {
		GENERICESP_SETTING(ImColor, background_color);
		GENERICESP_SETTING(float, spacing);
		GENERICESP_SETTING(float, width);
		GENERICESP_SETTING(ImColor, filled_color);
		GENERICESP_SETTING(ImColor, empty_color);
		GENERICESP_SETTING(bool, gradient);
		GENERICESP_SETTING(int, hue_steps);
		GENERICESP_SETTING(bool, flipped);
		GENERICESP_SETTING(bool, outlined);
		GENERICESP_SETTING(ImColor, outline_color);
		GENERICESP_SETTING(float, outline_thickness);

		using PercentageProvider = OpaqueLambda<float>;
		PercentageProvider percentage_provider;

		bool has_text = false; // True, when inherited by BarWithText

		explicit Bar(PercentageProvider percentage_provider);
		~Bar() override = default;

		// --- Likely irrelevant for users ---
		ImRect calculate_new_rect(const EntityType* e, const ImRect& rect) const;
		std::optional<ImRect> calculate_inner_rect(const EntityType* e, const ImRect& rect) const;
		ImRect calculate_bar_rect(const EntityType* e, ImRect rect, bool flipped, float percentage) const;
		// --- Likely irrelevant for users ---

		// --- Utility functions for color conversion ---
		using HsvColor = std::array<float, 3>;
		static HsvColor color_lerp(const HsvColor& from, const HsvColor& to, float t);
		static HsvColor color_rgb_to_hsv(ImColor color);
		static ImColor color_hsv_to_rgb(HsvColor hsv, float alpha);
		// --- Utility functions for color conversion ---

		void draw(ImDrawList* draw_list, const EntityType* e, UnionedRect& unioned_rect) const;
	};

	struct BarWithText : Bar, Text {
		GENERICESP_SETTING(bool, text_enabled);
		GENERICESP_SETTING(bool, hide_when_full);

		using Provider = OpaqueLambda<std::string>;
		Provider text_provider;

		explicit BarWithText(PercentageProvider percentage_provider, Provider text_provider);
		~BarWithText() override = default;

		void draw_number(ImDrawList* draw_list, const EntityType* e, const ImRect& bar_rect, float clamped_percentage) const;
		using Bar::draw;
	};

}
#endif
