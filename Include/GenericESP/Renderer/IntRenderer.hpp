#ifndef GENERICESP_RENDERER_INTRENDERER_HPP
#define GENERICESP_RENDERER_INTRENDERER_HPP

#include <functional>
#include <string>

#include "imgui.h"

namespace GenericESP {

	auto createIntRenderer(
		int min, int max, const std::function<void()>& onChange = [] {})
	{
		return [min, max, onChange](const std::string& id, int& thing) {
			if (ImGui::SliderInt(id.c_str(), &thing, min, max))
				onChange();
		};
	}
}

#endif
