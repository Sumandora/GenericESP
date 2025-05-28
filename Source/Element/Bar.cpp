#include <algorithm>
#include <stdexcept>
#define IMGUI_DEFINE_MATH_OPERATORS

#include "GenericESP/Element/Bar.hpp"

#include <utility>

using namespace GenericESP;

Bar::Bar(Bar::PercentageProvider percentageProvider, std::optional<NumberText::Provider> numberTextProvider)
	: percentageProvider(std::move(percentageProvider))
	, numberText(numberTextProvider.has_value() ? std::make_optional<NumberText>(*this, std::move(numberTextProvider.value())) : std::nullopt)
{
}

Bar::NumberText::NumberText(Bar& bar, NumberText::Provider provider)
	: bar(bar)
	, numberTextProvider(std::move(provider))
{
}

bool Bar::NumberText::get_enabled(const EntityType* e) const
{
	return bar.get_number_text_enabled(e);
}
float Bar::NumberText::get_font_scale(const EntityType* e) const
{
	return bar.get_number_text_font_scale(e);
}
ImColor Bar::NumberText::get_font_color(const EntityType* e) const
{
	return bar.get_number_text_font_color(e);
}
bool Bar::NumberText::get_shadow(const EntityType* e) const
{
	return bar.get_number_text_shadow(e);
}
float Bar::NumberText::get_shadow_offset(const EntityType* e) const
{
	return bar.get_number_text_shadow_offset(e);
}
ImColor Bar::NumberText::get_shadow_color(const EntityType* e) const
{
	return bar.get_number_text_shadow_color(e);
}
bool Bar::NumberText::get_hide_when_full(const EntityType* e) const
{
	return bar.get_number_text_hide_when_full(e);
}

void Bar::NumberText::draw(ImDrawList* drawList, const EntityType* e, ImVec2 pos) const
{
	Text::draw(drawList, e, numberTextProvider(e), pos, TextAlignment::CENTERED, VerticalAlignment::CENTERED);
}

ImRect Bar::calculateNewRect(const EntityType* e, const ImRect& rect) const
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float width = this->get_width(e);
	const float spacing = this->get_spacing(e);
#pragma clang diagnostic pop

	switch (get_side(e)) {
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

std::optional<ImRect> Bar::calculateInnerRect(const EntityType* e, const ImRect& rect) const
{
	if (this->get_outlined(e)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		const float outlineThickness = this->get_outline_thickness(e) / 2.0f;
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
ImRect Bar::calculateBarRect(const EntityType* e, ImRect rect, const bool flipped, const float percentage) const
#pragma clang diagnostic pop
{
	switch (get_side(e)) {
	case Side::LEFT:
	case Side::RIGHT:
		if (flipped)
			rect.Max.y = rect.Min.y + (rect.Max.y - rect.Min.y) * percentage;
		else
			rect.Min.y = rect.Max.y + (rect.Min.y - rect.Max.y) * percentage;
		break;
	case Side::TOP:
	case Side::BOTTOM:
		if (flipped)
			rect.Max.x = rect.Min.x + (rect.Max.x - rect.Min.x) * percentage;
		else
			rect.Min.x = rect.Max.x + (rect.Min.x - rect.Max.x) * percentage;
		break;
	default:
		throw std::runtime_error("Side was invalid");
	}

	return rect;
}

Bar::HsvColor Bar::colorLerp(const Bar::HsvColor& from, const Bar::HsvColor& to, float t)
{
	return {
		from[0] + (to[0] - from[0]) * t,
		from[1] + (to[1] - from[1]) * t,
		from[2] + (to[2] - from[2]) * t
	};
}

Bar::HsvColor Bar::colorRGBtoHSV(ImColor color)
{
	Bar::HsvColor hsv{};
	ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hsv[0], hsv[1], hsv[2]);
	return hsv;
}

ImColor Bar::colorHSVtoRGB(Bar::HsvColor hsv)
{
	ImColor rgb{ 0.0f, 0.0f, 0.0f, 1.0f };
	ImGui::ColorConvertHSVtoRGB(hsv[0], hsv[1], hsv[2], rgb.Value.x, rgb.Value.y, rgb.Value.z);
	return rgb;
}

void Bar::draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const
{
	const Side side = get_side(e);

	ImRect& rect = chooseRect(e, unionedRect);

	const ImRect newRect = calculateNewRect(e, rect);

	rect = ImRect{
		{ std::min(rect.Min.x, newRect.Min.x),
			std::min(rect.Min.y, newRect.Min.y) },
		{ std::max(rect.Max.x, newRect.Max.x),
			std::max(rect.Max.y, newRect.Max.y) },
	};

	const std::optional<ImRect> innerRectOpt = calculateInnerRect(e, newRect);

	if (get_outlined(e)) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		float outlineThickness = this->get_outline_thickness(e);
		if (const float width = this->get_width(e); outlineThickness > width)
#pragma clang diagnostic pop
			outlineThickness = width;
		const ImVec2 shrinkage{
			outlineThickness / 2.0f,
			outlineThickness / 2.0f
		};
		drawList->AddRect(newRect.Min + shrinkage, newRect.Max - shrinkage, get_outline_color(e), 0.0f,
			ImDrawFlags_None, outlineThickness);
	}

	if (innerRectOpt.has_value()) {
		const ImRect innerRect = innerRectOpt.value();

		const float percentage = percentageProvider(e);
		const float clampedPercentage = std::clamp(percentage, 0.0f, 1.0f);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		const bool flipped = this->get_flipped(e);
		const bool gradient = this->get_gradient(e);

		ImColor filledColor = this->get_filled_color(e);
		ImColor emptyColor = this->get_empty_color(e);
#pragma clang diagnostic pop

		if (gradient && flipped) // To prevent the parameters passed to AddRectFilledMultiColor from varying, just flip the colors once
			std::swap(filledColor, emptyColor);

		const ImRect barRect = calculateBarRect(e, innerRect, flipped, clampedPercentage);
		const ImRect complementBarRect = calculateBarRect(e, innerRect, !flipped, 1.0f - clampedPercentage);

		drawList->AddRectFilled(complementBarRect.Min, complementBarRect.Max, get_background_color(e));

		auto emptyHsv = colorRGBtoHSV(emptyColor);
		auto filledHsv = colorRGBtoHSV(filledColor);

		if (gradient) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			int hueSteps = this->get_hue_steps(e);
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

				const auto middleHsv = colorLerp(filledHsv, emptyHsv, t);
				const float alpha = filledColor.Value.w + (emptyColor.Value.w - filledColor.Value.w) * t;

				const auto middleRgb = colorHSVtoRGB(middleHsv);
				const ImColor middleColor(middleRgb.Value.x, middleRgb.Value.y, middleRgb.Value.z, alpha);

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
			const auto middleHsv = colorLerp(emptyHsv, filledHsv, clampedPercentage);
			const auto middleRgb = colorHSVtoRGB(middleHsv);
			const float alpha = filledColor.Value.w + (emptyColor.Value.w - filledColor.Value.w) * clampedPercentage;

			const ImColor middleColor(middleRgb.Value.x, middleRgb.Value.y, middleRgb.Value.z, alpha);

			drawList->AddRectFilled(barRect.Min, barRect.Max, middleColor);
		}

		if (numberText.has_value()) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			const auto& numberText = this->numberText.value();
#pragma clang diagnostic pop
			if (!numberText.get_hide_when_full(e) || clampedPercentage < 1.0f) {
				auto textPosition = [&]() -> ImVec2 {
					switch (side) {
					case Side::TOP:
					case Side::BOTTOM: {
						return { flipped ? barRect.Max.x : barRect.Min.x, barRect.Min.y + (barRect.Max.y - barRect.Min.y) * 0.5f };
					}
					case Side::LEFT:
					case Side::RIGHT: {
						return { barRect.Min.x + (barRect.Max.x - barRect.Min.x) * 0.5f, flipped ? barRect.Max.y : barRect.Min.y };
					}
					}
					std::unreachable();
				}();
				numberText.draw(drawList, e, textPosition);
			}
		}
	}
}
