#ifndef GENERICESP_SERIALIZATION_IMCOLORSERIALIZATION_HPP
#define GENERICESP_SERIALIZATION_IMCOLORSERIALIZATION_HPP

#include "Serialization.hpp"
#include "imgui.h"

namespace GenericESP {
	inline SerializedTypeMap serializeImColor(const ImColor& color) {
		SerializedTypeMap map;

		map["Red"] = color.Value.x;
		map["Green"] = color.Value.y;
		map["Blue"] = color.Value.z;
		map["Alpha"] = color.Value.w;

		return map;
	}

	inline void deserializeImColor(const SerializedTypeMap& map, ImColor& thing) {
		auto red = map.get<float>("Red");
		auto green = map.get<float>("Green");
		auto blue = map.get<float>("Blue");
		auto alpha = map.get<float>("Alpha");

		if (!red.has_value() ||
			!green.has_value() ||
			!blue.has_value() ||
			!alpha.has_value())
			return;

		thing = {
			red.value(),
			green.value(),
			blue.value(),
			alpha.value()
		};
	}
}

#endif
