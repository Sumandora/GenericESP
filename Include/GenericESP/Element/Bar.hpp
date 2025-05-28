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

		struct NumberText : Text {
			Bar& bar;

			using Provider = OpaqueLambda<std::string>;
			Provider number_text_provider;

			explicit NumberText(Bar& parent, Provider provider);
			~NumberText() override = default;

			void draw(ImDrawList* draw_list, const EntityType* e, ImVec2 pos) const;

			virtual bool get_enabled(const EntityType* e) const;

			float get_font_scale(const EntityType* e) const override;
			ImColor get_font_color(const EntityType* e) const override;
			bool get_shadow(const EntityType* e) const override;
			float get_shadow_offset(const EntityType* e) const override;
			ImColor get_shadow_color(const EntityType* e) const override;

			virtual bool get_hide_when_full(const EntityType* e) const;
		};

		GENERICESP_SETTING(bool, number_text_enabled);

		GENERICESP_SETTING(float, number_text_font_scale);
		GENERICESP_SETTING(ImColor, number_text_font_color);
		GENERICESP_SETTING(bool, number_text_shadow);
		GENERICESP_SETTING(float, number_text_shadow_offset);
		GENERICESP_SETTING(ImColor, number_text_shadow_color);

		GENERICESP_SETTING(bool, number_text_hide_when_full);

		std::optional<NumberText> number_text;

		explicit Bar(PercentageProvider percentage_provider, std::optional<NumberText::Provider> number_text_provider = std::nullopt);
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
		static ImColor color_hsv_to_rgb(HsvColor hsv);
		// --- Utility functions for color conversion ---

		void draw(ImDrawList* draw_list, const EntityType* e, UnionedRect& unioned_rect) const;
	};

}
#endif
