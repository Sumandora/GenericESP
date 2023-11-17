#ifndef GENERICESP_RENDERER_BOOLRENDERER_HPP
#define GENERICESP_RENDERER_BOOLRENDERER_HPP

#include <functional>
#include <string>

#include "imgui.h"

namespace GenericESP {

	auto createBoolRenderer(const std::function<void()>& onChange = [] {})
	{
		return [onChange](const std::string& id, bool& thing) {
			if (ImGui::Checkbox(id.c_str(), &thing))
				onChange();
		};
	}

}

#endif
