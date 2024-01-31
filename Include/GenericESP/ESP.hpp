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

		/**
		 * True if there is no way that any element draws (Always false when Element#enabled is a DynamicConfig)
		 * This can be used to prevent unnecessary loops through all entities to check if any of them have an enabled config
		 */
		[[nodiscard]] bool isDefinitelyDisabled() const;
		/**
		 * True if 'e' has elements that want to render on them
		 * This can be used to check if the ESP would have effect on an entity
		 */
		[[nodiscard]] bool isEnabled(const EntityType* e) const;

		void renderGui();
		[[nodiscard]] SerializedTypeMap serialize() const;
		void deserialize(const SerializedTypeMap& map);
	};
}

#endif
