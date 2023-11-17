#ifndef GENERICESP_ELEMENT_LINE_HPP
#define GENERICESP_ELEMENT_LINE_HPP

#include "../Renderer/ColorRenderer.hpp"
#include "../Renderer/FloatRenderer.hpp"
#include "Element.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct Line : Element<EntityType> {
		using Element<EntityType>::enabled;
		MixableConfigurableValue<ImColor, EntityType> lineColor{
			"Line color",
			StaticConfig<ImColor>{ { 1.0f, 1.0f, 1.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<float, EntityType> thickness{
			"Thickness",
			StaticConfig<float>{ 1.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") }
		};
		MixableConfigurableValue<bool, EntityType> outlined{ "Outlined", StaticConfig<bool>{ true, createBoolRenderer() } };
		MixableConfigurableValue<ImColor, EntityType> outlineColor{
			"Outline color",
			StaticConfig<ImColor>{ { 0.0f, 0.0f, 0.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<float, EntityType> outlineThickness{
			"Outline thickness",
			StaticConfig<float>{ 1.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") }
		};

		void draw(ImDrawList* drawList, const EntityType& e, const std::vector<ImVec2>& points)
		{
			if (!enabled(e))
				return;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			const float thickness = this->thickness(e);
#pragma clang diagnostic pop

			if (outlined(e))
				drawList->AddPolyline(points.data(), (int)points.size(), outlineColor(e), ImDrawFlags_None, thickness + outlineThickness(e));

			drawList->AddPolyline(points.data(), (int)points.size(), lineColor(e), ImDrawFlags_None, thickness);
		}

		void renderGui(const std::string& id)
		{
			ImGui::PushID(id.c_str());
			enabled.renderGui();
			lineColor.renderGui();
			thickness.renderGui();
			outlined.renderGui();
			outlineColor.renderGui();
			outlineThickness.renderGui();
			ImGui::PopID();
		}
	};

}

#endif
