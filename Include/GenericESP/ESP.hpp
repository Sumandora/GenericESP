#ifndef GENERICESP_ESP_HPP
#define GENERICESP_ESP_HPP

#include <functional>
#include <list>
#include <string>

#include "GenericESP/Serialization/Serialization.hpp"

struct ImColor;

namespace GenericESP {

	struct Element;

	// This is syntactic sugar to indicate that you can use opaque lambdas here
	// If you are intended to pass a getter lambda to a function, and it takes a EntityType parameter
	// then you can use the Util#makeOpaque function to auto cast back and forth between your entity type and the
	// entity type used by the internals of this project.
	// Check the example for more information
	using EntityType = void;

	struct ESP {
		std::vector<Element*> elements;

		void renderGui();
		[[nodiscard]] SerializedTypeMap serialize() const;
		void deserialize(const SerializedTypeMap& map);
	};
}

#endif
