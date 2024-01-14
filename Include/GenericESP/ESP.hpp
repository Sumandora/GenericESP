#ifndef GENERICESP_ESP_HPP
#define GENERICESP_ESP_HPP

#include <list>
#include <functional>
#include <string>

struct ImColor;

namespace GenericESP {

	// This is syntactic sugar to indicate that you can use opaque lambdas here
	// If you are intended to pass a getter lambda to a function, and it takes a EntityType parameter
	// then you can use the Util#makeOpaque function to auto cast back and forth between your entity type and the
	// entity type used by the internals of this project.
	// Check the example for more information
	using EntityType = void;

	class ESP {
	public:
		virtual std::function<void(const std::string&, bool&)> createBoolRenderer(const std::function<void()>& onChange = [] {}) = 0;
		virtual std::function<void(const std::string&, ImColor&)> createColorRenderer(const std::function<void()>& onChange = [] {}) = 0;
		virtual std::function<void(const std::string&, std::size_t&)> createComboRenderer(const std::initializer_list<std::string>& localization, const std::function<void()>& onChange = [] {}) = 0;
		virtual std::function<void(const std::string&, float&)> createFloatRenderer(float min, float max, const char* fmt, const std::function<void()>& onChange = [] {}) = 0;
		virtual std::function<void(const std::string&, int&)> createIntRenderer(int min, int max, const std::function<void()>& onChange = [] {}) = 0;
	};
}

#endif
