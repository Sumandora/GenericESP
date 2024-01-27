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

	using BoolRenderer	= std::function<void(const std::string&, bool&)>;
	using ColorRenderer	= std::function<void(const std::string&, ImColor&)>;
	using ComboRenderer	= std::function<void(const std::string&, std::size_t&)>;
	using FloatRenderer	= std::function<void(const std::string&, float&)>;
	using IntRenderer	= std::function<void(const std::string&, int&)>;

	using ChangeCallback = std::function<void()>;

	struct ESP {
		std::vector<std::unique_ptr<Element>> elements;

		virtual BoolRenderer	createBoolRenderer(const ChangeCallback& onChange = [] {}) = 0;
		virtual ColorRenderer	createColorRenderer(const ChangeCallback& onChange = [] {}) = 0;
		virtual ComboRenderer	createComboRenderer(const std::initializer_list<std::string>& localization, const ChangeCallback& onChange = [] {}) = 0;
		virtual FloatRenderer	createFloatRenderer(float min, float max, const char* fmt, const ChangeCallback& onChange = [] {}) = 0;
		virtual IntRenderer		createIntRenderer(int min, int max, const ChangeCallback& onChange = [] {}) = 0;

		void renderGui();
		[[nodiscard]] SerializedTypeMap serialize() const;
		void deserialize(const SerializedTypeMap& map);
	};
}

#endif
