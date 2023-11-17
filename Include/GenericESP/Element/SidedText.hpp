#ifndef GENERICESP_ELEMENT_SIDEDTEXT_HPP
#define GENERICESP_ELEMENT_SIDEDTEXT_HPP

#include "SidedElement.hpp"
#include "Text.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct SidedText : SidedElement<EntityType> {
		using SidedElement<EntityType>::enabled;
		using SidedElement<EntityType>::side;
		MixableConfigurableValue<float, EntityType> spacing{
			"Spacing",
			StaticConfig<float>{ 1.0f, createFloatRenderer(0.0, 10.0f, "%.2f") }
		};
		Text<EntityType> textElement;

		SidedText()
			: SidedElement<EntityType>(Side::TOP)
		{
			// Remove second enabled setting
			textElement.enabled.options = DynamicConfig<bool, EntityType>{
				[this](const EntityType& e) { return this->enabled(e); },
				[](const std::string& id) { /* don't render anything */ }
			};
		}

		using SidedElement<EntityType>::chooseRect;

		void draw(ImDrawList* drawList, const EntityType& e, const std::string& text, UnionedRect& unionedRect)
		{
			if (!enabled(e))
				return;

			const Side side = this->side(e);

			ImRect& rect = chooseRect(side, unionedRect);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			const float spacing = this->spacing(e);
#pragma clang diagnostic pop

			switch (side) {
			case Side::TOP: {
				auto size = textElement.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Min.y - spacing }, TextAlignment::CENTERED, VerticalAlignment::ABOVE_POSITION);
				if (size.has_value())
					rect.Min.y -= size->y + spacing;
				break;
			}
			case Side::LEFT: {
				auto size = textElement.draw(drawList, e, text, { rect.Min.x - spacing, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.5f }, TextAlignment::RIGHT_BOUNDED, VerticalAlignment::CENTERED);
				if (size.has_value())
					rect.Min.x -= size->x + spacing;
				break;
			}
			case Side::BOTTOM: {
				auto size = textElement.draw(drawList, e, text, { rect.Min.x + (rect.Max.x - rect.Min.x) * 0.5f, rect.Max.y + spacing }, TextAlignment::CENTERED, VerticalAlignment::BELOW_POSITION);
				if (size.has_value())
					rect.Max.y += size->y + spacing;
				break;
			}
			case Side::RIGHT: {
				auto size = textElement.draw(drawList, e, text, { rect.Max.x + spacing, rect.Min.y + (rect.Max.y - rect.Min.y) * 0.5f }, TextAlignment::LEFT_BOUNDED, VerticalAlignment::CENTERED);
				if (size.has_value())
					rect.Max.x += size->x + spacing;
				break;
			}
			}
		}

		void renderGui(const std::string& id)
		{
			ImGui::PushID(id.c_str());
			enabled.renderGui();
			side.renderGui();
			spacing.renderGui();
			textElement.renderGui("Text");
			ImGui::PopID();
		}
	};

}

#endif
