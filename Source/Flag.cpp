#include "GenericESP/Flag.hpp"

using namespace GenericESP;

Flag::Flag(GenericESP::ESP* base, std::string&& name, GenericESP::Flag::Remaps&& remaps, std::string&& defaultFormat)
	: name(std::move(name))
	, textElement(base)
	, remaps(std::move(remaps))
	, format(std::move(defaultFormat))
{
}

std::string Flag::computeText(const EntityType* e) const
{
	std::string result = format;
	for (const auto& [varName, provider] : remaps) {
		size_t pos = 0;
		while ((pos = result.find('%' + varName + '%', pos)) != std::string::npos) {
			std::string replacement = provider(e);
			result.replace(pos, varName.length() + 2, replacement);
			pos += replacement.length();
		}
	}
	return result;
}

void Flag::renderGui(const std::string& id)
{
	ImGui::PushID(id.c_str());
	textElement.renderGui("Text element");
	ImGui::InputText("Formatting", (char*)format.c_str(), format.capacity() + 1, ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
		auto* format = reinterpret_cast<std::string*>(data->UserData);
#pragma clang diagnostic pop
		format->resize(data->BufTextLen);
		data->Buf = (char*)format->c_str();
		return 0;
	}, &format); // from imgui_stdlib
	ImGui::PopID();
}
