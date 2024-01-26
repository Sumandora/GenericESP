#include "GenericESP/Serialization/ImColorSerialization.hpp"

using namespace GenericESP;

SerializedTypeMap GenericESP::serializeImColor(const ImColor& color) {
	SerializedTypeMap map;

	map["Red"] = color.Value.x;
	map["Green"] = color.Value.y;
	map["Blue"] = color.Value.z;
	map["Alpha"] = color.Value.w;

	return map;
}

void GenericESP::deserializeImColor(const SerializedTypeMap& map, ImColor& thing) {
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
