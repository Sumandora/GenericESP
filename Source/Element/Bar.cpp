#define IMGUI_DEFINE_MATH_OPERATORS
#include "GenericESP/Element/Element.hpp"
#include "GenericESP/Element/SidedElement.hpp"
#include "GenericESP/Element/Text.hpp"
#include "GenericESP/UnionedRect.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <algorithm>
#include <optional>
#include <stdexcept>

#include "GenericESP/Element/Bar.hpp"

#include <utility>

using namespace GenericESP;

Bar::Bar(Bar::PercentageProvider percentage_provider, std::optional<NumberText::Provider> number_text_provider)
	: percentage_provider(std::move(percentage_provider))
	, number_text(number_text_provider.has_value() ? std::make_optional<NumberText>(*this, std::move(number_text_provider.value())) : std::nullopt)
{
}

Bar::NumberText::NumberText(Bar& parent, NumberText::Provider provider)
	: bar(parent)
	, number_text_provider(std::move(provider))
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

void Bar::NumberText::draw(ImDrawList* draw_list, const EntityType* e, ImVec2 pos) const
{
	Text::draw(draw_list, e, number_text_provider(e), pos, TextAlignment::CENTERED, VerticalAlignment::CENTERED);
}

ImRect Bar::calculate_new_rect(const EntityType* e, const ImRect& rect) const
{
	const float width = this->get_width(e);
	const float spacing = this->get_spacing(e);

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

std::optional<ImRect> Bar::calculate_inner_rect(const EntityType* e, const ImRect& rect) const
{
	if (this->get_outlined(e)) {
		const float outline_thickness = this->get_outline_thickness(e) / 2.0F;

		const ImVec2 shrinkage{
			outline_thickness,
			outline_thickness
		};
		const ImVec2 top_left = rect.Min + shrinkage;
		const ImVec2 bottom_right = rect.Max - shrinkage;

		if (top_left.x > bottom_right.x || top_left.y > bottom_right.y) // If overlapped
			return std::nullopt;

		return ImRect{ top_left, bottom_right };
	}

	return rect;
}

ImRect Bar::calculate_bar_rect(const EntityType* e, ImRect rect, const bool flipped, const float percentage) const
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

Bar::HsvColor Bar::color_lerp(const Bar::HsvColor& from, const Bar::HsvColor& to, float t)
{
	return {
		from[0] + (to[0] - from[0]) * t,
		from[1] + (to[1] - from[1]) * t,
		from[2] + (to[2] - from[2]) * t
	};
}

Bar::HsvColor Bar::color_rgb_to_hsv(ImColor color)
{
	Bar::HsvColor hsv{};
	ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hsv[0], hsv[1], hsv[2]);
	return hsv;
}

ImColor Bar::color_hsv_to_rgb(Bar::HsvColor hsv)
{
	ImColor rgb{ 0.0F, 0.0F, 0.0F, 1.0F };
	ImGui::ColorConvertHSVtoRGB(hsv[0], hsv[1], hsv[2], rgb.Value.x, rgb.Value.y, rgb.Value.z);
	return rgb;
}

void Bar::draw(ImDrawList* draw_list, const EntityType* e, UnionedRect& unioned_rect) const
{
	const Side side = get_side(e);

	ImRect& rect = choose_rect(e, unioned_rect);

	const ImRect new_rect = calculate_new_rect(e, rect);

	rect = ImRect{
		{ std::min(rect.Min.x, new_rect.Min.x),
			std::min(rect.Min.y, new_rect.Min.y) },
		{ std::max(rect.Max.x, new_rect.Max.x),
			std::max(rect.Max.y, new_rect.Max.y) },
	};

	const std::optional<ImRect> inner_rect_opt = calculate_inner_rect(e, new_rect);

	if (get_outlined(e)) {
		float outline_thickness = this->get_outline_thickness(e);
		outline_thickness = std::min(outline_thickness, get_width(e));
		const ImVec2 shrinkage{
			outline_thickness / 2.0F,
			outline_thickness / 2.0F
		};
		draw_list->AddRect(new_rect.Min + shrinkage, new_rect.Max - shrinkage, get_outline_color(e), 0.0F,
			ImDrawFlags_None, outline_thickness);
	}

	if (inner_rect_opt.has_value()) {
		const ImRect inner_rect = inner_rect_opt.value();

		const float percentage = percentage_provider(e);
		const float clamped_percentage = std::clamp(percentage, 0.0F, 1.0F);

		const bool flipped = this->get_flipped(e);
		const bool gradient = this->get_gradient(e);

		ImColor filled_color = this->get_filled_color(e);
		ImColor empty_color = this->get_empty_color(e);

		if (gradient && flipped) // To prevent the parameters passed to AddRectFilledMultiColor from varying, just flip the colors once
			std::swap(filled_color, empty_color);

		const ImRect bar_rect = calculate_bar_rect(e, inner_rect, flipped, clamped_percentage);
		const ImRect complement_bar_rect = calculate_bar_rect(e, inner_rect, !flipped, 1.0F - clamped_percentage);

		draw_list->AddRectFilled(complement_bar_rect.Min, complement_bar_rect.Max, get_background_color(e));

		auto empty_hsv = color_rgb_to_hsv(empty_color);
		auto filled_hsv = color_rgb_to_hsv(filled_color);

		if (gradient) {
			const int hue_steps = this->get_hue_steps(e);

			// Draw multiple small rectangle because ImGui lerps RGB, not HSV. (<-- This is common)
			// If you want to see what it would look like without this (forcefully) set hueSteps to 2.

			float prev_t = flipped ? 0.0F : 1.0F;
			ImColor prev_color = filled_color;

			const auto max_hue_steps = static_cast<float>(hue_steps - 1);
			for (int i = 0; i < hue_steps; i++) {
				float t = (static_cast<float>(i) / max_hue_steps) * clamped_percentage;
				if (!flipped) {
					t = 1.0F - t;
				}

				const auto middle_hsv = color_lerp(filled_hsv, empty_hsv, t);
				const float alpha = filled_color.Value.w + (empty_color.Value.w - filled_color.Value.w) * t;

				const auto middle_rgb = color_hsv_to_rgb(middle_hsv);
				const ImColor middle_color(middle_rgb.Value.x, middle_rgb.Value.y, middle_rgb.Value.z, alpha);

				switch (side) {
				case Side::TOP:
				case Side::BOTTOM: {
					const ImRect partial_rect = ImRect{
						{ inner_rect.Min.x + (inner_rect.Max.x - inner_rect.Min.x) * t,
							inner_rect.Min.y },
						{ inner_rect.Min.x + (inner_rect.Max.x - inner_rect.Min.x) * prev_t,
							inner_rect.Max.y }
					};
					draw_list->AddRectFilledMultiColor(partial_rect.Min, partial_rect.Max, middle_color, prev_color, prev_color, middle_color);
					break;
				}
				case Side::LEFT:
				case Side::RIGHT: {
					const ImRect partial_rect = ImRect{
						{ inner_rect.Min.x,
							inner_rect.Min.y + (inner_rect.Max.y - inner_rect.Min.y) * prev_t },
						{ inner_rect.Max.x,
							inner_rect.Min.y + (inner_rect.Max.y - inner_rect.Min.y) * t }
					};
					draw_list->AddRectFilledMultiColor(partial_rect.Min, partial_rect.Max, prev_color, prev_color, middle_color, middle_color);
					break;
				}
				}

				prev_t = t;
				prev_color = middle_color;
			}

		} else {
			const auto middle_hsv = color_lerp(empty_hsv, filled_hsv, clamped_percentage);
			const auto middle_rgb = color_hsv_to_rgb(middle_hsv);
			const float alpha = filled_color.Value.w + (empty_color.Value.w - filled_color.Value.w) * clamped_percentage;

			const ImColor middle_color(middle_rgb.Value.x, middle_rgb.Value.y, middle_rgb.Value.z, alpha);

			draw_list->AddRectFilled(bar_rect.Min, bar_rect.Max, middle_color);
		}

		if (number_text.has_value()) {
			const auto& number_text = this->number_text.value();
			if (!number_text.get_hide_when_full(e) || clamped_percentage < 1.0F) {
				auto text_position = [&]() -> ImVec2 {
					switch (side) {
					case Side::TOP:
					case Side::BOTTOM: {
						return { flipped ? bar_rect.Max.x : bar_rect.Min.x, bar_rect.Min.y + (bar_rect.Max.y - bar_rect.Min.y) * 0.5F };
					}
					case Side::LEFT:
					case Side::RIGHT: {
						return { bar_rect.Min.x + (bar_rect.Max.x - bar_rect.Min.x) * 0.5F, flipped ? bar_rect.Max.y : bar_rect.Min.y };
					}
					}
					std::unreachable();
				}();
				number_text.draw(draw_list, e, text_position);
			}
		}
	}
}
