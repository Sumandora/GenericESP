#ifndef GENERICESP_ELEMENT_CIRCLE_HPP
#define GENERICESP_ELEMENT_CIRCLE_HPP

#include "../Renderer/ColorRenderer.hpp"
#include "../Renderer/FloatRenderer.hpp"
#include "Element.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct Circle : Element<EntityType> {
		using Element<EntityType>::enabled;
		MixableConfigurableValue<ImColor, EntityType> circleColor{
			"Circle color",
			StaticConfig<ImColor>{ { 1.0f, 1.0f, 1.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<float, EntityType> radius{
			"Radius",
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

		void draw(ImDrawList* drawList, const EntityType& e, const ImVec2& position)
		{
			if (!enabled(e))
				return;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			const float radius = this->radius(e);
#pragma clang diagnostic pop

			if (outlined(e))
				drawList->AddCircleFilled(position, radius + outlineThickness(e), outlineColor(e));

			drawList->AddCircleFilled(position, radius, circleColor(e));
		}

		void renderGui(const std::string& id)
		{
			ImGui::PushID(id.c_str());
			enabled.renderGui();
			circleColor.renderGui();
			radius.renderGui();
			outlined.renderGui();
			outlineColor.renderGui();
			outlineThickness.renderGui();
			ImGui::PopID();
		}
	};

}

#endif