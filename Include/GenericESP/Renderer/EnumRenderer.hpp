#ifndef GENERICESP_RENDERER_ENUMRENDERER_HPP
#define GENERICESP_RENDERER_ENUMRENDERER_HPP

#include <functional>
#include <string>
#include <unordered_map>

namespace GenericESP {

	template <typename E>
	auto createEnumComboRenderer(
		const std::unordered_map<E, std::string>& localization, const std::function<void()>& onChange = [] {})
	{
		return [&localization, onChange](const std::string& id, E& thing) {
			const char* names[localization.size()];
			for (int i = 0; i < localization.size(); i++) {
				names[i] = localization.at(static_cast<E>(i)).c_str();
			}

			int i = static_cast<int>(thing);
			if (ImGui::Combo(id.c_str(), &i, names, IM_ARRAYSIZE(names))) {
				thing = static_cast<E>(i);
				onChange();
			}
		};
	}
}

#endif
