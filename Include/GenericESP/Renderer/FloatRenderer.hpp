#ifndef GENERICESP_RENDERER_FLOATRENDERER_HPP
#define GENERICESP_RENDERER_FLOATRENDERER_HPP

#include <functional>
#include <string>

#include "imgui.h"

namespace GenericESP {

	auto createFloatRenderer(
		float min, float max, const char* fmt, const std::function<void()>& onChange = [] {})
	{
		return [min, max, fmt, onChange](const std::string& id, float& thing) {
			if (ImGui::SliderFloat(id.c_str(), &thing, min, max, fmt))
				onChange();
		};
	}

}

#endif
