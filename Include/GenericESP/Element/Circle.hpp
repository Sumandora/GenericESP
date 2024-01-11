#ifndef GENERICESP_ELEMENT_CIRCLE_HPP
#define GENERICESP_ELEMENT_CIRCLE_HPP

#include "../Renderer/ColorRenderer.hpp"
#include "../Renderer/FloatRenderer.hpp"
#include "Element.hpp"

namespace GenericESP {

	struct Circle : Element {
		using Element::enabled;
		MixableConfigurableValue<ImColor> circleColor{
			"Circle color",
			StaticConfig<ImColor>{ { 1.0f, 1.0f, 1.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<float> radius{
			"Radius",
			StaticConfig<float>{ 1.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") }
		};
		MixableConfigurableValue<bool> outlined{ "Outlined", StaticConfig<bool>{ true, createBoolRenderer() } };
		MixableConfigurableValue<ImColor> outlineColor{
			"Outline color",
			StaticConfig<ImColor>{ { 0.0f, 0.0f, 0.0f, 1.0f }, createColorRenderer() }
		};
		MixableConfigurableValue<float> outlineRadius{
			"Outline radius",
			StaticConfig<float>{ 2.0f, createFloatRenderer(0.0f, 10.0f, "%.2f") }
		};

		void draw(ImDrawList* drawList, const void* e, const ImVec2& position)
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
