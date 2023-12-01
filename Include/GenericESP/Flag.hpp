#ifndef GENERICESP_FLAG_HPP
#define GENERICESP_FLAG_HPP

#include "Element/Text.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct Flag {
		std::string name;

		Text<EntityType> textElement;

		using Provider = std::function<std::string(const EntityType&)>;
		using Remaps = std::unordered_map<std::string, Provider>;

		Remaps remaps;
		std::string format;
		explicit Flag(std::string name, Remaps remaps, std::string defaultFormat)
			: name(std::move(name))
			, textElement({})
			, remaps(std::move(remaps))
			, format(std::move(defaultFormat))
		{
		}

		std::string computeText(const EntityType& e) const
		{
			std::string result = format;
			for (const auto& [varName, provider] : remaps) {
				size_t pos = 0;
				while ((pos = result.find("%" + varName + "%", pos)) != std::string::npos) {
					std::string replacement = provider(e);
					result.replace(pos, varName.length() + 2, replacement);
					pos += replacement.length();
				}
			}
			return result;
		}

		void renderGui(const std::string& id)
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
	};

}

#endif
