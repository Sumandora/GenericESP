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

		using PercentageProvider = std::function<float(const EntityType*)>;
		PercentageProvider percentageProvider;

		struct NumberText {
			Text numberText;
			bool hideWhenFull;

			using Provider = std::function<std::string(const EntityType*)>;
			Provider numberTextProvider;

			explicit NumberText(ESP* base, Provider&& provider);

			void draw(ImDrawList* drawList, const EntityType* e, ImVec2 pos) const;
			void renderGui(const std::string& id);
		};

		std::optional<NumberText> numberText;

		explicit Bar(ESP* base, PercentageProvider&& percentageProvider, std::optional<NumberText> numberText = std::nullopt);

		// --- Likely irrelevant for users ---
		ImRect calculateNewRect(const EntityType* e, const ImRect& rect) const;
		std::optional<ImRect> calculateInnerRect(const EntityType* e, const ImRect& rect) const;
		ImRect calculateBarRect(const EntityType* e, ImRect rect, bool flipped, float percentage) const;
		// --- Likely irrelevant for users ---

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const;
		void renderGui(const std::string& id);
	};

}
#endif
