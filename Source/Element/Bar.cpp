#define IMGUI_DEFINE_MATH_OPERATORS

#include "GenericESP/Element/Bar.hpp"
#include "GenericESP/Serialization/ImColorSerialization.hpp"

#include <utility>

using namespace GenericESP;

Bar::Bar(ESP* base, std::string id, Bar::PercentageProvider percentageProvider, std::optional<NumberText> numberText)
	: SidedElement(base, std::move(id), Side::LEFT)
	, backgroundColor{ StaticConfig<ImColor>{ "Background color", { 0.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor } }
	, spacing{ StaticConfig<float>{ "Spacing", 1.0f, base->createFloatRenderer(0.0, 10.0f, "%.2f") } }
	, width{ StaticConfig<float>{ "Width", 2.0f, base->createFloatRenderer(0.0, 10.0f, "%.2f") } }
	, filledColor{ StaticConfig<ImColor>{ "Filled color", { 0.0f, 1.0f, 0.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor } }
	, emptyColor{ StaticConfig<ImColor>{ "Empty color", { 1.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor } }
	, hueSteps{ StaticConfig<int>{ "Hue steps", 3, base->createIntRenderer(3, 10) }, [this] {
				   const ConfigurableValue<bool>& selected = gradient.getSelected();
				   return !selected.isStatic() || selected.getStaticConfig().thing;
			   } }
	, flipped{ StaticConfig<bool>{ "Flipped", false, base->createBoolRenderer() } }
	, gradient{ StaticConfig<bool>{ "Gradient", true, base->createBoolRenderer() } }
	, outlined{ StaticConfig<bool>{ "Outlined", true, base->createBoolRenderer() } }
	, outlineColor{ StaticConfig<ImColor>{ "Outline color", { 0.0f, 0.0f, 0.0f, 1.0f }, base->createColorRenderer(), serializeImColor, deserializeImColor }, [this] {
					   const ConfigurableValue<bool>& selected = outlined.getSelected();
					   return !selected.isStatic() || selected.getStaticConfig().thing;
				   } }
	, outlineThickness{ StaticConfig<float>{ "Outline thickness", 1.0f, base->createFloatRenderer(0.0, 10.0f, "%.2f") }, [this] {
						   const ConfigurableValue<bool>& selected = outlined.getSelected();
						   return !selected.isStatic() || selected.getStaticConfig().thing;
					   } }
	, percentageProvider(std::move(percentageProvider))
	, numberText(std::move(numberText))
{
}

Bar::NumberText::NumberText(ESP* base, NumberText::Provider provider)
	: numberTextProvider(std::move(provider))
	, numberText(base, "Number text")
	, hideWhenFull(false)
{
	id = numberText.id;
}

void Bar::NumberText::draw(ImDrawList* drawList, const EntityType* e, ImVec2 pos) const
{
	numberText.draw(drawList, e, numberTextProvider(e), pos, TextAlignment::CENTERED, VerticalAlignment::CENTERED);
}

void Bar::NumberText::renderGui()
{
	const auto popupLabel = numberText.id + "##Popup";

	ImGui::Text("%s", numberText.id.c_str());
	ImGui::SameLine();
	if (ImGui::Button("..."))
		ImGui::OpenPopup(popupLabel.c_str());

	if (ImGui::BeginPopup(popupLabel.c_str())) {
		numberText.renderGui();
		// Hide when full will be rendered through the injected render method
		ImGui::EndPopup();
	}
}

SerializedTypeMap Bar::NumberText::serialize() const
{
	SerializedTypeMap map;
	map.putSubtree("Number text", numberText.serialize());
	map["Hide when full"] = hideWhenFull;
	return map;
}

void Bar::NumberText::deserialize(const SerializedTypeMap& map)
{
	auto numberTextOpt = map.getSubtree("Number text");
	if(numberTextOpt.has_value())
		numberText.deserialize(numberTextOpt.value());
	auto hideWhenFullOpt = map.get<bool>("Hide when full");
	if (hideWhenFullOpt.has_value())
		hideWhenFull = hideWhenFullOpt.value();
}

ImRect Bar::calculateNewRect(const EntityType* e, const ImRect& rect) const
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
	const float width = this->width(e);
	const float spacing = this->spacing(e);
#pragma clang diagnostic pop

	switch (getSide(e)) {
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
ImRect Bar::calculateBarRect(const EntityType* e, ImRect rect, const bool flipped, const float percentage) const
#pragma clang diagnostic pop
{
	switch (getSide(e)) {
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
	if (!enabled(e))
		return;

	const Side side = getSide(e);

	ImRect& rect = chooseRect(e, unionedRect);

	const ImRect newRect = calculateNewRect(e, rect);

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
#pragma clang diagnostic pop

		if (gradient && flipped) // To prevent the parameters passed to AddRectFilledMultiColor from varying, just flip the colors once
			std::swap(filledColor, emptyColor);

		const ImRect barRect = calculateBarRect(e, innerRect, flipped, clampedPercentage);
		const ImRect complementBarRect = calculateBarRect(e, innerRect, !flipped, 1.0f - clampedPercentage);

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

		if (numberText.has_value() && numberText->numberText.enabled(e))
			if (!numberText->hideWhenFull || clampedPercentage < 1.0f) {
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
				numberText->draw(drawList, e, textPosition);
			}
	}
}

void Bar::renderGui()
{
	ImGui::PushID(id.c_str());
	for (Renderable* r : std::initializer_list<Renderable*>{
			 &enabled, &side, &backgroundColor, &spacing,
			 &width, &filledColor, &emptyColor, &gradient,
			 &hueSteps, &flipped, &outlined, &outlineColor,
			 &outlineThickness })
		r->renderGui();
	if (numberText.has_value())
		numberText->renderGui();
	ImGui::PopID();
}

SerializedTypeMap Bar::serialize() const
{
	SerializedTypeMap map;
	for (const MixableBase* mixable : std::initializer_list<const MixableBase*>{
			 &enabled, &side, &backgroundColor, &spacing,
			 &width, &filledColor, &emptyColor, &gradient,
			 &hueSteps, &flipped, &outlined, &outlineColor,
			 &outlineThickness })
		mixable->serialize(map);
	if (numberText.has_value())
		map.putSubtree(numberText->id, numberText->serialize());
	return map;
}

void Bar::deserialize(const SerializedTypeMap& map)
{
	for (MixableBase* mixable : std::initializer_list<MixableBase*>{
			 &enabled, &side, &backgroundColor, &spacing,
			 &width, &filledColor, &emptyColor, &gradient,
			 &hueSteps, &flipped, &outlined, &outlineColor,
			 &outlineThickness })
		mixable->deserializeFromParent(map);
	if (numberText.has_value() && map.contains(numberText->id)) {
		auto opt = map.getSubtree(numberText->id);
		if(opt.has_value())
			numberText->deserialize(opt.value());
	}
}