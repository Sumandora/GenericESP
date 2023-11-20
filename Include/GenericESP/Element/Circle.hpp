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
		MixableConfigurableValue<float, EntityType> outlineRadius{
			"Outline radius",
			StaticConfig<float>{ 2.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") }
		};

		void draw(ImDrawList* drawList, const EntityType& e, const ImVec2& position)
		{
			if (!enabled(e))
				return;

			if (outlined(e))
				drawList->AddCircleFilled(position, outlineRadius(e), outlineColor(e));

			drawList->AddCircleFilled(position, radius(e), circleColor(e));
		}

		void renderGui(const std::string& id)
		{
			ImGui::PushID(id.c_str());
			enabled.renderGui();
			circleColor.renderGui();
			radius.renderGui();
			outlined.renderGui();
			outlineColor.renderGui();
			outlineRadius.renderGui();
			ImGui::PopID();
		}
	};

}

#endif
