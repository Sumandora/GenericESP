#include "GenericESP/DefaultRenderers.hpp"

using namespace GenericESP;

BoolRenderer DefaultRenderers::_createBoolRenderer(const ChangeCallback& onChange)
{
	return [onChange](const std::string& id, bool& thing) {
		if (ImGui::Checkbox(id.c_str(), &thing))
			onChange();
	};
}

ColorRenderer DefaultRenderers::_createColorRenderer(const ChangeCallback& onChange)
{
	return [onChange](const std::string& id, ImColor& thing) {
		ImGui::PushID(id.c_str());
		const bool clicked = ImGui::ColorButton(id.c_str(), thing.Value, ImGuiColorEditFlags_None, ImVec2(0, 0));
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

ComboRenderer DefaultRenderers::_createComboRenderer(const std::vector<std::string>& localization, const ChangeCallback& onChange)
{
	return [localization, onChange](const std::string& id, std::size_t& thing) {
		const char* names[localization.size()];
		std::size_t idx = 0;
		for (const auto& local : localization) {
			names[idx] = local.c_str();
			idx++;
		}

		int i = static_cast<int>(thing);
		if (ImGui::Combo(id.c_str(), &i, names, IM_ARRAYSIZE(names))) {
			thing = static_cast<std::size_t>(i);
			onChange();
		}
	};
}

FloatRenderer DefaultRenderers::_createFloatRenderer(float min, float max, const char* fmt, const ChangeCallback& onChange)
{
	return [min, max, fmt, onChange](const std::string& id, float& thing) {
		if (ImGui::SliderFloat(id.c_str(), &thing, min, max, fmt))
			onChange();
	};
}

IntRenderer DefaultRenderers::_createIntRenderer(int min, int max, const ChangeCallback& onChange)
{
	return [min, max, onChange](const std::string& id, int& thing) {
		if (ImGui::SliderInt(id.c_str(), &thing, min, max))
			onChange();
	};
}

TextRenderer DefaultRenderers::_createTextRenderer(const GenericESP::ChangeCallback& onChange) {
	return [onChange](const std::string& id, std::string& thing) {
		if(ImGui::InputText("Formatting", (char*)thing.c_str(), thing.capacity() + 1, ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
			auto* thing = reinterpret_cast<std::string*>(data->UserData);
#pragma clang diagnostic pop
			thing->resize(data->BufTextLen);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "LocalValueEscapesScope"
			data->Buf = (char*)thing->c_str();
#pragma clang diagnostic pop
			return 0;
		}, &thing)) // from imgui_stdlib
			onChange();
	};
}

ButtonRenderer DefaultRenderers::_createButtonRenderer() {
	return [](const std::string& id) {
		return ImGui::Button(id.c_str());
	};
}
