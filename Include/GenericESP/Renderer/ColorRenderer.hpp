#ifndef GENERICESP_RENDERER_COLORRENDERER_HPP
#define GENERICESP_RENDERER_COLORRENDERER_HPP

#include <functional>
#include <string>

#include "imgui.h"

namespace GenericESP {

	auto createColorRenderer(const std::function<void()>& onChange = [] {})
	{
		static ImColor colorClipboard; // TODO How would I make this user interoperable?
		return [onChange](const std::string& id, ImColor& thing) {
			ImGui::PushID(id.c_str());
			const bool clicked = ImGui::ColorButton(id.c_str(), thing.Value, ImGuiColorEditFlags_None, ImVec2(0, 0));
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Selectable("Copy")) {
					colorClipboard = thing;
				}
				if (ImGui::Selectable("Paste")) {
					thing = colorClipboard;
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();
			ImGui::Text("%s", id.c_str());

			if (clicked)
				ImGui::OpenPopup("##Picker");

			if (ImGui::BeginPopup("##Picker")) {
				float color[] = { thing.Value.x, thing.Value.y, thing.Value.z, thing.Value.w };
				if (ImGui::ColorPicker4(id.c_str(), color, 0)) {
					thing.Value = ImVec4(color[0], color[1], color[2], color[3]);
					onChange();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		};
	}
}

#endif
