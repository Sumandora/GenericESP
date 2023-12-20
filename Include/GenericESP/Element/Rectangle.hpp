#ifndef GENERICESP_ELEMENT_RECTANGLE_HPP
#define GENERICESP_ELEMENT_RECTANGLE_HPP

#define IMGUI_DEFINE_MATH_OPERATORS

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
			StaticConfig<float>{ 1.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") }
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
				2.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") },
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

			const float totalWidth = outlined ? std::max(thickness, outlineThickness) : thickness;
			const float halfWidth = totalWidth / 2;

			const ImVec2 min = rect.getMin();
			const ImVec2 max = rect.getMax();

			if (outlined)
				drawList->AddRect(min, max, outlineColor(e), rounding, ImDrawFlags_None,
					outlineThickness);

			if (fill(e)) {
				const float halfThickness = thickness / 2.0f;
				const ImVec2 rectWidth{ halfThickness, halfThickness };
				drawList->AddRectFilled(min + rectWidth, max - rectWidth, fillColor(e), rounding, ImDrawFlags_None);
			}

			drawList->AddRect(min, max, color(e), rounding, ImDrawFlags_None, thickness);

			rect.expand(halfWidth);
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

		[[nodiscard]] Serialization serialize() const override {
			Serialization serialization;
			serialization["Enabled"] = enabled.serialize();
			serialization["Color"] = color.serialize();
			serialization["Thickness"] = thickness.serialize();
			serialization["Outlined"] = outlined.serialize();
			serialization["Outline color"] = outlineColor.serialize();
			serialization["Outline thickness"] = outlineThickness.serialize();
			serialization["Fill"] = fill.serialize();
			serialization["Fill color"] = fillColor.serialize();
			return serialization;
		}

		void deserialize(const Serialization& data) override {
			enabled.deserialize(std::get<Serialization>(data["Enabled"]));
			color.deserialize(std::get<Serialization>(data["Color"]));
			thickness.deserialize(std::get<Serialization>(data["Thickness"]));
			outlined.deserialize(std::get<Serialization>(data["Outlined"]));
			outlineColor.deserialize(std::get<Serialization>(data["Outline color"]));
			outlineThickness.deserialize(std::get<Serialization>(data["Outline thickness"]));
			fill.deserialize(std::get<Serialization>(data["Fill"]));
			fillColor.deserialize(std::get<Serialization>(data["Fill color"]));
		}
	};

}

#endif
