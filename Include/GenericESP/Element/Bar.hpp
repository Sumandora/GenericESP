#ifndef GENERICESP_ELEMENT_BAR_HPP
#define GENERICESP_ELEMENT_BAR_HPP

#include "SidedElement.hpp"
#include "Text.hpp"
#include "../OpaqueLambda.hpp"

namespace GenericESP {

	struct Bar : SidedElement {
		Mixable<ImColor> backgroundColor;
		Mixable<float> spacing;
		Mixable<float> width;
		Mixable<ImColor> filledColor;
		Mixable<ImColor> emptyColor;
		Mixable<bool> gradient;
		Mixable<int> hueSteps;
		Mixable<bool> flipped;
		Mixable<bool> outlined;
		Mixable<ImColor> outlineColor;
		Mixable<float> outlineThickness;

		using PercentageProvider = OpaqueLambda<float>;
		PercentageProvider percentageProvider;

		struct NumberText : Serializable {
			std::string id;

			Text numberText;
			bool hideWhenFull;

			using Provider = OpaqueLambda<std::string>;
			Provider numberTextProvider;

			explicit NumberText(ESP* base, Provider provider);

			void draw(ImDrawList* drawList, const EntityType* e, ImVec2 pos) const;
			void renderGui();

			[[nodiscard]] SerializedTypeMap serialize() const override;
			void deserialize(const SerializedTypeMap& map) override;
		};

		std::optional<NumberText> numberText;

		explicit Bar(ESP* base, std::string id, PercentageProvider percentageProvider, std::optional<NumberText::Provider> numberTextProvider = std::nullopt);

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
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap& map) override;
	};

}
#endif
