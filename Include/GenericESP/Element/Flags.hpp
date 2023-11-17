#ifndef GENERICESP_ELEMENT_FLAGS_HPP
#define GENERICESP_ELEMENT_FLAGS_HPP

#include "../Flag.hpp"
#include "../VectorOrdering.hpp"
#include "SidedElement.hpp"

#include <memory>
#include <ranges>

namespace GenericESP {

	template <typename EntityType, typename... Ts>
		requires std::conjunction_v<std::is_base_of<Flag<EntityType>, Ts>...>
	struct Flags : SidedElement<EntityType> {
		using SidedElement<EntityType>::enabled;
		using SidedElement<EntityType>::side;
		MixableConfigurableValue<float, EntityType> spacing{
			"Spacing",
			StaticConfig<float>{ 1.0f, createFloatRenderer(0.0, 10.0f, "%.2f") }
		};

		std::vector<std::unique_ptr<Flag<EntityType>>> flags;
		VectorOrdering<std::unique_ptr<Flag<EntityType>>> flagOrder;

		explicit Flags()
			: SidedElement<EntityType>(Side::RIGHT)
			, flagOrder{ flags, [](const std::unique_ptr<Flag<EntityType>>& flag) { return flag->name; } }
		{
			(flags.emplace_back(std::make_unique<Ts>()), ...);
		}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		ImVec2 drawEntry(Side side, Text<EntityType> textElement, ImDrawList* drawList, const EntityType& e, ImRect& rect, std::string text, float spacing, float yOffset)
		{
#pragma clang diagnostic pop
			switch (side) {
			case Side::TOP: {
				auto size = textElement.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Min.y - spacing - yOffset }, TextAlignment::CENTERED, VerticalAlignment::ABOVE_POSITION);
				if (size.has_value())
#pragma clang diagnostic push
#pragma ide diagnostic ignored "ArgumentSelectionDefects"
					return { yOffset + size->y, size->y };
#pragma clang diagnostic pop
			}
			case Side::LEFT: {
				auto size = textElement.draw(drawList, e, text, { rect.Min.x - spacing, rect.Min.y + yOffset }, TextAlignment::RIGHT_BOUNDED, VerticalAlignment::BELOW_POSITION);
				if (size.has_value())
					return size.value();
			}
			case Side::BOTTOM: {
				auto size = textElement.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Max.y + spacing + yOffset }, TextAlignment::CENTERED, VerticalAlignment::BELOW_POSITION);
				if (size.has_value())
#pragma clang diagnostic push
#pragma ide diagnostic ignored "ArgumentSelectionDefects"
					return { yOffset + size->y, size->y };
#pragma clang diagnostic pop
			}
			case Side::RIGHT: {
				auto size = textElement.draw(drawList, e, text, { rect.Max.x + spacing, rect.Min.y + yOffset }, TextAlignment::LEFT_BOUNDED, VerticalAlignment::BELOW_POSITION);
				if (size.has_value())
					return size.value();
			}
			}
			return { 0.0f, 0.0f };
		}

		using SidedElement<EntityType>::chooseRect;

		void draw(ImDrawList* drawList, const EntityType& e, UnionedRect& unionedRect)
		{
			if (!enabled(e))
				return;

			const Side side = this->side(e);

			ImRect& rect = chooseRect(side, unionedRect);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			const float spacing = this->spacing(e);
#pragma clang diagnostic pop

			float biggestOffset = 0.0f;
			float yOffset = 0.0f;

			auto process = [&](const std::unique_ptr<Flag<EntityType>>& flag) {
				ImVec2 offset = drawEntry(side, flag->textElement, drawList, e, rect, flag->computeText(e), spacing, yOffset);
				if (offset.x > biggestOffset)
					biggestOffset = offset.x;
				yOffset += offset.y;
			};

			if (side == Side::TOP) { // We render from bottom to top in this case, so reverse the flags to get the same order
				for (const auto& flag : std::ranges::views::reverse(flags))
					process(flag);
			} else
				for (const auto& flag : flags)
					process(flag);

			biggestOffset += spacing;

			switch (side) {
			case Side::TOP:
				rect.Min.y -= biggestOffset;
				break;
			case Side::LEFT:
				rect.Min.x -= biggestOffset;
				break;
			case Side::BOTTOM:
				rect.Max.y += biggestOffset;
				break;
			case Side::RIGHT:
				rect.Max.x += biggestOffset;
				break;
			}
		}

		void renderGui(const std::string& id)
		{
			ImGui::PushID(id.c_str());
			enabled.renderGui();
			side.renderGui();
			spacing.renderGui();
			if (ImGui::BeginTabBar("Flags", ImGuiTabBarFlags_Reorderable)) {
				for (std::unique_ptr<Flag<EntityType>>& flag : flags) {
					const char* flagName = flag->name.c_str();
					if (ImGui::BeginTabItem(flagName)) {
						flag->renderGui(flagName);
						ImGui::EndTabItem();
					}
				}
				if (ImGui::BeginTabItem("Order")) {
					flagOrder.renderGui("Order");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::PopID();
		}
	};

}

#endif
