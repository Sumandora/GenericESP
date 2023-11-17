#ifndef GENERICESP_UTIL_HPP
#define GENERICESP_UTIL_HPP

#include <array>
#include <cstdint>

#include "imgui.h"

namespace GenericESP {

	template <typename T, std::size_t N>
	std::array<T, N> arrayLerp(const std::array<T, N>& from, const std::array<T, N>& to, float t)
	{
		std::array<T, N> newArray{};
		for (int i = 0; i < N; i++) {
			newArray[i] = from[i] + (to[i] - from[i]) * t;
		}
		return newArray;
	}

	std::array<float, 3> colorRGBtoHSV(ImColor color)
	{
		std::array<float, 3> hsv{};
		ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hsv[0], hsv[1], hsv[2]);
		return hsv;
	}

	std::array<float, 3> colorHSVtoRGB(std::array<float, 3> hsv)
	{
		std::array<float, 3> rgb{};
		ImGui::ColorConvertHSVtoRGB(hsv[0], hsv[1], hsv[2], rgb[0], rgb[1], rgb[2]);
		return rgb;
	}
}

#endif
