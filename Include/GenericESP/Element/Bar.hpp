#ifndef GENERICESP_ELEMENT_BAR_HPP
#define GENERICESP_ELEMENT_BAR_HPP

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../Util.hpp"
#include "SidedElement.hpp"
#include "Text.hpp"

namespace GenericESP {

	struct Bar : SidedElement {
		MixableConfigurableValue<ImColor> backgroundColor;
		MixableConfigurableValue<float> spacing;
		MixableConfigurableValue<float> width;
		MixableConfigurableValue<ImColor> filledColor;
		MixableConfigurableValue<ImColor> emptyColor;
		MixableConfigurableValue<bool> gradient;
		MixableConfigurableValue<int> hueSteps;
		MixableConfigurableValue<bool> flipped;
		MixableConfigurableValue<bool> outlined;
		MixableConfigurableValue<ImColor> outlineColor;
		MixableConfigurableValue<float> outlineThickness;

		using PercentageProvider = std::function<float(const void*)>;
		PercentageProvider percentageProvider;

		struct NumberText {
			Text numberText;
			bool hideWhenFull;

			using Provider = std::function<std::string(const void*)>;
			Provider numberTextProvider;

			explicit NumberText(ESP* base, Provider&& provider);

			void draw(ImDrawList* drawList, const void* e, ImVec2 pos) const;
			void renderGui(const std::string& id);
		};

		std::optional<NumberText> numberText;

		explicit Bar(ESP* base, PercentageProvider&& percentageProvider, std::optional<NumberText> numberText = std::nullopt);

		ImRect calculateNewRect(const void* e, const ImRect& rect) const;
		std::optional<ImRect> calculateInnerRect(const void* e, const ImRect& rect) const;
		ImRect calculateBarRect(const void* e, ImRect rect, bool flipped, float percentage) const;
		void draw(ImDrawList* drawList, const void* e, UnionedRect& unionedRect) const;
		void renderGui(const std::string& id);
	};

}
#endif
