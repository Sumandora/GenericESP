#ifndef GENERICESP_ELEMENT_BAR_HPP
#define GENERICESP_ELEMENT_BAR_HPP

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../Renderer/IntRenderer.hpp"
#include "../Util.hpp"
#include "SidedElement.hpp"
#include "Text.hpp"

namespace GenericESP {

	struct Bar : SidedElement {
		using SidedElement::enabled;
		using SidedElement::side;
		MixableConfigurableValue<ImColor> backgroundColor{
			"Background color",
			StaticConfig<ImColor>{ { 0.0f, 0.0f, 0.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<float> spacing{
			"Spacing",
			StaticConfig<float>{ 1.0f, createFloatRenderer(0.0, 10.0f, "%.2f") }
		};
		MixableConfigurableValue<float> width{
			"Width",
			StaticConfig<float>{ 2.0f, createFloatRenderer(0.0, 10.0f, "%.2f") }
		};
		MixableConfigurableValue<ImColor> filledColor{
			"Filled color",
			StaticConfig<ImColor>{ { 0.0f, 1.0f, 0.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<ImColor> emptyColor{
			"Empty color",
			StaticConfig<ImColor>{ { 1.0f, 0.0f, 0.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<bool> gradient{ "Gradient", StaticConfig<bool>{ true, createBoolRenderer() } };
		MixableConfigurableValue<int> hueSteps{
			"Hue steps",
			StaticConfig<int>{ 3, createIntRenderer(3, 10) },
			[this] {
				const ConfigurableValue<bool>& selected = gradient.getSelected();
				return !selected.isStatic() || selected.getStaticConfig().thing;
			}
		};
		MixableConfigurableValue<bool> flipped{ "Flipped", StaticConfig<bool>{ false, createBoolRenderer() } };
		MixableConfigurableValue<bool> outlined{ "Outlined", StaticConfig<bool>{ true, createBoolRenderer() } };
		MixableConfigurableValue<ImColor> outlineColor{
			"Outline color",
			StaticConfig<ImColor>{ { 0.0f, 0.0f, 0.0f, 1.0f }, createColorRenderer() },
			[this] {
				const ConfigurableValue<bool>& selected = outlined.getSelected();
				return !selected.isStatic() || selected.getStaticConfig().thing;
			}
		};
		MixableConfigurableValue<float> outlineThickness{
			"Outline thickness",
			StaticConfig<float>{ 1.0f, createFloatRenderer(0.0, 10.0f, "%.2f") },
			[this] {
				const ConfigurableValue<bool>& selected = outlined.getSelected();
				return !selected.isStatic() || selected.getStaticConfig().thing;
			}
		};

		using PercentageProvider = std::function<float(const void* e)>;
		PercentageProvider percentageProvider;

		struct NumberText {
			Text numberText{};
			bool hideWhenFull = false;

			using Provider = std::function<std::string(const void* e)>;
			Provider numberTextProvider;

			std::function<void(const std::string&, bool&)> boolRenderer = createBoolRenderer();
			explicit NumberText(Provider provider)
				: numberTextProvider(std::move(provider))
			{
				StaticConfig<bool>& enabledCfg = numberText.enabled.getSelected().getStaticConfig();
				auto oldRenderer = enabledCfg.render;
				enabledCfg.render = [this, oldRenderer](const std::string& id, bool& thing) {
					oldRenderer(id, thing);
					if (thing) {
						ImGui::SameLine();
						boolRenderer("Hide when full", hideWhenFull);
					}
				};
			}

			void draw(ImDrawList* drawList, const void* e, ImVec2 pos)
			{
				numberText.draw(drawList, e, numberTextProvider(e), pos, TextAlignment::CENTERED, VerticalAlignment::CENTERED);
			}

			void renderGui(const std::string& id)
			{
				const auto popupLabel = id + "##Popup";

				ImGui::Text("%s", id.c_str());
				ImGui::SameLine();
				if (ImGui::Button("..."))
					ImGui::OpenPopup(popupLabel.c_str());

				if (ImGui::BeginPopup(popupLabel.c_str())) {
					numberText.renderGui(id);
					ImGui::EndPopup();
				}
				// Hide when full will be rendered through the injected render method
			}
		};

		std::optional<NumberText> numberText;

		explicit Bar(PercentageProvider percentageProvider,
			const std::optional<typename NumberText::Provider>& numberTextProvider = std::nullopt)
			: SidedElement(Side::LEFT)
			, percentageProvider(std::move(percentageProvider))
			, numberText(numberTextProvider.has_value()
					  ? std::optional<NumberText>{ numberTextProvider.value() }
					  : std::nullopt)
		{
		}

		ImRect calculateNewRect(Side side, const void* e, const ImRect& rect) const
		{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			const float width = this->width(e);
			const float spacing = this->spacing(e);
#pragma clang diagnostic pop

			switch (side) {
			case Side::TOP:
				return ImRect{
					{ rect.Min.x,
						rect.Min.y - spacing - width },
					{ rect.Max.x,
						rect.Min.y - spacing }
				};
			case Side::LEFT:
				return ImRect{
					{ rect.Min.x - spacing - width,
						rect.Min.y },
					{ rect.Min.x - spacing,
						rect.Max.y }
				};
			case Side::BOTTOM:
				return ImRect{
					{ rect.Min.x,
						rect.Max.y + spacing },
					{ rect.Max.x,
						rect.Max.y + spacing + width }
				};
			case Side::RIGHT:
				return ImRect{
					{ rect.Max.x + spacing,
						rect.Min.y },
					{ rect.Max.x + spacing + width,
						rect.Max.y }
				};
			default:
				throw std::runtime_error("Side was invalid");
			}
		}

		std::optional<ImRect> calculateInnerRect(const void* e, const ImRect& rect) const
		{
			if (this->outlined(e)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
				const float outlineThickness = this->outlineThickness(e) / 2.0f;
#pragma clang diagnostic pop

				const ImVec2 shrinkage{
					outlineThickness,
					outlineThickness
				};
				const ImVec2 topLeft = rect.Min + shrinkage;
				const ImVec2 bottomRight = rect.Max - shrinkage;

				if (topLeft.x > bottomRight.x || topLeft.y > bottomRight.y) // If overlapped
					return std::nullopt;

				return ImRect{ topLeft, bottomRight };
			}

			return rect;
		}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		static ImRect calculateBarRect(Side side, const ImRect& rect, const bool flipped, const float percentage)
		{
#pragma clang diagnostic pop
			ImRect barRect = rect;
			switch (side) {
			case Side::LEFT:
			case Side::RIGHT:
				if (flipped)
					barRect.Max.y = barRect.Min.y + (barRect.Max.y - barRect.Min.y) * percentage;
				else
					barRect.Min.y = barRect.Max.y + (barRect.Min.y - barRect.Max.y) * percentage;
				break;
			case Side::TOP:
			case Side::BOTTOM:
				if (flipped)
					barRect.Max.x = barRect.Min.x + (barRect.Max.x - barRect.Min.x) * percentage;
				else
					barRect.Min.x = barRect.Max.x + (barRect.Min.x - barRect.Max.x) * percentage;
				break;
			default:
				throw std::runtime_error("Side was invalid");
			}

			return barRect;
		}

		using SidedElement::chooseRect;

		void draw(ImDrawList* drawList, const void* e, UnionedRect& unionedRect)
		{
			if (!enabled(e))
				return;

			const Side side = this->side(e);

			ImRect& rect = chooseRect(side, unionedRect);

			const ImRect newRect = calculateNewRect(side, e, rect);

			rect = ImRect{
				{ std::min(rect.Min.x, newRect.Min.x),
					std::min(rect.Min.y, newRect.Min.y) },
				{ std::max(rect.Max.x, newRect.Max.x),
					std::max(rect.Max.y, newRect.Max.y) },
			};

			const std::optional<ImRect> innerRectOpt = calculateInnerRect(e, newRect);

			if (outlined(e)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
				float outlineThickness = this->outlineThickness(e);
				if (const float width = this->width(e); outlineThickness > width)
#pragma clang diagnostic pop
					outlineThickness = width;
				const ImVec2 shrinkage{
					outlineThickness / 2.0f,
					outlineThickness / 2.0f
				};
				drawList->AddRect(newRect.Min + shrinkage, newRect.Max - shrinkage, outlineColor(e), 0.0f,
					ImDrawFlags_None, outlineThickness);
			}

			if (innerRectOpt.has_value()) {
				const ImRect innerRect = innerRectOpt.value();

				const float percentage = percentageProvider(e);
				const float clampedPercentage = std::clamp(percentage, 0.0f, 1.0f);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
				const bool flipped = this->flipped(e);
				const bool gradient = this->gradient(e);

				ImColor filledColor = this->filledColor(e);
				ImColor emptyColor = this->emptyColor(e);

				if (gradient && flipped) // To prevent the parameters passed to AddRectFilledMultiColor from varying, just flip the colors once
					std::swap(filledColor, emptyColor);
#pragma clang diagnostic pop

				const ImRect barRect = calculateBarRect(side, innerRect, flipped, clampedPercentage);
				const ImRect complementBarRect = calculateBarRect(side, innerRect, !flipped, 1.0f - clampedPercentage);

				drawList->AddRectFilled(complementBarRect.Min, complementBarRect.Max, backgroundColor(e));

				auto emptyHsv = colorRGBtoHSV(emptyColor);
				auto filledHsv = colorRGBtoHSV(filledColor);

				if (gradient) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
					int hueSteps = this->hueSteps(e);
#pragma clang diagnostic pop

					// Draw multiple small rectangle because ImGui lerps RGB, not HSV. (<-- This is common)
					// If you want to see what it would look like without this (forcefully) set hueSteps to 2.

					float prevT = flipped ? 0.0f : 1.0f;
					ImColor prevColor = filledColor;

					const auto maxHueSteps = static_cast<float>(hueSteps - 1);
					for (unsigned int i = 0; i < hueSteps; i++) {
						float t = (static_cast<float>(i) / maxHueSteps) * clampedPercentage;
						if (!flipped) {
							t = 1.0f - t;
						}

						const auto middleHsv = arrayLerp(filledHsv, emptyHsv, t);
						const float alpha = filledColor.Value.w + (emptyColor.Value.w - filledColor.Value.w) * t;

						const auto middleRgb = colorHSVtoRGB(middleHsv);
						const ImColor middleColor(middleRgb[0], middleRgb[1], middleRgb[2], alpha);

						switch (side) {
						case Side::TOP:
						case Side::BOTTOM: {
							ImRect partialRect = ImRect{
								{ innerRect.Min.x + (innerRect.Max.x - innerRect.Min.x) * t,
									innerRect.Min.y },
								{ innerRect.Min.x + (innerRect.Max.x - innerRect.Min.x) * prevT,
									innerRect.Max.y }
							};
							drawList->AddRectFilledMultiColor(partialRect.Min, partialRect.Max, middleColor, prevColor, prevColor, middleColor);
							break;
						}
						case Side::LEFT:
						case Side::RIGHT: {
							ImRect partialRect = ImRect{
								{ innerRect.Min.x,
									innerRect.Min.y + (innerRect.Max.y - innerRect.Min.y) * prevT },
								{ innerRect.Max.x,
									innerRect.Min.y + (innerRect.Max.y - innerRect.Min.y) * t }
							};
							drawList->AddRectFilledMultiColor(partialRect.Min, partialRect.Max, prevColor, prevColor, middleColor, middleColor);
							break;
						}
						}

						prevT = t;
						prevColor = middleColor;
					}

				} else {
					const auto middleHsv = arrayLerp(emptyHsv, filledHsv, clampedPercentage);
					const auto middleRgb = colorHSVtoRGB(middleHsv);
					const float alpha = filledColor.Value.w + (emptyColor.Value.w - filledColor.Value.w) * clampedPercentage;

					const ImColor middleColor(middleRgb[0], middleRgb[1], middleRgb[2], alpha);

					drawList->AddRectFilled(barRect.Min, barRect.Max, middleColor);
				}

				if (numberText.has_value() && numberText->numberText.enabled(e))
					if (!numberText->hideWhenFull || clampedPercentage < 1.0f) {
						switch (side) {
						case Side::TOP:
						case Side::BOTTOM: {
							numberText->draw(drawList, e, { flipped ? barRect.Max.x : barRect.Min.x, barRect.Min.y + (barRect.Max.y - barRect.Min.y) * 0.5f });
							break;
						}
						case Side::LEFT:
						case Side::RIGHT: {
							numberText->draw(drawList, e, { barRect.Min.x + (barRect.Max.x - barRect.Min.x) * 0.5f, flipped ? barRect.Max.y : barRect.Min.y });
							break;
						}
						}
					}
			}
		}

		void renderGui(const std::string& id)
		{
			ImGui::PushID(id.c_str());
			enabled.renderGui();
			side.renderGui();
			backgroundColor.renderGui();
			spacing.renderGui();
			width.renderGui();
			filledColor.renderGui();
			emptyColor.renderGui();
			gradient.renderGui();
			hueSteps.renderGui();
			flipped.renderGui();
			outlined.renderGui();
			outlineColor.renderGui();
			outlineThickness.renderGui();
			if (numberText.has_value())
				numberText->renderGui("Number text");
			ImGui::PopID();
		}
	};

}
#endif
