#ifndef GENERICESP_ELEMENT_BAR_HPP
#define GENERICESP_ELEMENT_BAR_HPP

#include "../OpaqueLambda.hpp"
#include "GenericESP/Element/Element.hpp"
#include "SidedElement.hpp"
#include "Text.hpp"

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
		PercentageProvider percentageProvider;

		struct NumberText : Text {
			Bar& bar;

			using Provider = OpaqueLambda<std::string>;
			Provider numberTextProvider;

			explicit NumberText(Bar& parent, Provider provider);
			virtual ~NumberText() = default;

			void draw(ImDrawList* drawList, const EntityType* e, ImVec2 pos) const;

			virtual bool get_enabled(const EntityType*) const;

			float get_font_scale(const EntityType*) const override;
			ImColor get_font_color(const EntityType*) const override;
			bool get_shadow(const EntityType*) const override;
			float get_shadow_offset(const EntityType*) const override;
			ImColor get_shadow_color(const EntityType*) const override;

			virtual bool get_hide_when_full(const EntityType*) const;
		};

		GENERICESP_SETTING(bool, number_text_enabled);

		GENERICESP_SETTING(float, number_text_font_scale);
		GENERICESP_SETTING(ImColor, number_text_font_color);
		GENERICESP_SETTING(bool, number_text_shadow);
		GENERICESP_SETTING(float, number_text_shadow_offset);
		GENERICESP_SETTING(ImColor, number_text_shadow_color);

		GENERICESP_SETTING(bool, number_text_hide_when_full);

		std::optional<NumberText> numberText;

		explicit Bar(PercentageProvider percentageProvider, std::optional<NumberText::Provider> numberTextProvider = std::nullopt);

		// --- Likely irrelevant for users ---
		ImRect calculateNewRect(const EntityType* e, const ImRect& rect) const;
		std::optional<ImRect> calculateInnerRect(const EntityType* e, const ImRect& rect) const;
		ImRect calculateBarRect(const EntityType* e, ImRect rect, bool flipped, float percentage) const;
		// --- Likely irrelevant for users ---

		// --- Utility functions for color conversion ---
		using HsvColor = std::array<float, 3>;
		static HsvColor colorLerp(const HsvColor& from, const HsvColor& to, float t);
		static HsvColor colorRGBtoHSV(ImColor color);
		static ImColor colorHSVtoRGB(HsvColor hsv);
		// --- Utility functions for color conversion ---

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const;
	};

}
#endif
