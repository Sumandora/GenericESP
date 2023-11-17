#ifndef GENERICESP_ELEMENT_RECTANGLE_HPP
#define GENERICESP_ELEMENT_RECTANGLE_HPP

#include "../Renderer/ColorRenderer.hpp"
#include "../Renderer/FloatRenderer.hpp"
#include "../UnionedRect.hpp"
#include "Element.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct Rectangle : Element<EntityType> {
		using Element<EntityType>::enabled;
		MixableConfigurableValue<ImColor, EntityType> color{
			"Color",
			StaticConfig<ImColor>{
				{ 1.0f, 1.0f, 1.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<float, EntityType> rounding{
			"Rounding",
			StaticConfig<float>{ 0.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") }
		};
		MixableConfigurableValue<float, EntityType> thickness{
			"Thickness",
			StaticConfig<float>{ 2.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") }
		};
		MixableConfigurableValue<bool, EntityType> outlined{ "Outlined", StaticConfig<bool>{ true, createBoolRenderer() } };
		MixableConfigurableValue<ImColor, EntityType> outlineColor{
			"Outline color",
			StaticConfig<ImColor>{
				{ 0.0f, 0.0f, 0.0f, 1.0f }, createColorRenderer() },
			[this] {
				const ConfigurableValue<bool, EntityType>& selected = outlined.getSelected();
				return !selected.isStatic() || selected.getStaticConfig().thing;
			}
		};
		MixableConfigurableValue<float, EntityType> outlineThickness{
			"Outline thickness",
			StaticConfig<float>{
				1.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") },
			[this] {
				const ConfigurableValue<bool, EntityType>& selected = outlined.getSelected();
				return !selected.isStatic() || selected.getStaticConfig().thing;
			}
		};
		MixableConfigurableValue<bool, EntityType> fill{ "Fill", StaticConfig<bool>{ false, createBoolRenderer() } };
		MixableConfigurableValue<ImColor, EntityType> fillColor{
			"Fill color",
			StaticConfig<ImColor>{
				{ 1.0f, 1.0f, 1.0f, 1.0f }, createColorRenderer() },
			[this] {
				const ConfigurableValue<bool, EntityType>& selected = fill.getSelected();
				return !selected.isStatic() || selected.getStaticConfig().thing;
			}
		};

		void draw(ImDrawList* drawList, const EntityType& e, UnionedRect& rect)
		{
			if (!enabled(e))
				return;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			const float rounding = this->rounding(e);

			const float thickness = this->thickness(e);
			const bool outlined = this->outlined(e);
			const float outlineThickness = this->outlineThickness(e);
#pragma clang diagnostic pop

			const ImVec2 min = rect.getMin();
			const ImVec2 max = rect.getMax();

			if (outlined)
				drawList->AddRect(min, max, outlineColor(e), rounding, ImDrawFlags_None,
					outlineThickness);

			if (fill(e))
				drawList->AddRectFilled(min, max, fillColor(e), rounding, ImDrawFlags_None);
			else
				drawList->AddRect(min, max, color(e), rounding, ImDrawFlags_None, thickness);

			const float expansion = std::max(thickness, outlined ? outlineThickness : 0.0f) / 2;

			rect.expand(expansion);
		}

		void renderGui(const std::string& id)
		{
			ImGui::PushID(id.c_str());
			enabled.renderGui();
			color.renderGui();
			rounding.renderGui();
			thickness.renderGui();
			outlined.renderGui();
			outlineColor.renderGui();
			outlineThickness.renderGui();
			fill.renderGui();
			fillColor.renderGui();
			ImGui::PopID();
		}
	};

}

#endif
