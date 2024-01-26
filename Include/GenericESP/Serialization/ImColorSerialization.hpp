#ifndef GENERICESP_SERIALIZATION_IMCOLORSERIALIZATION_HPP
#define GENERICESP_SERIALIZATION_IMCOLORSERIALIZATION_HPP

#include "Serialization.hpp"
#include "imgui.h"

namespace GenericESP {
	SerializedTypeMap serializeImColor(const ImColor& color);
	void deserializeImColor(const SerializedTypeMap& map, ImColor& thing);
}

#endif
