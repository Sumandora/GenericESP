#ifndef GENERICESP_ESP_HPP
#define GENERICESP_ESP_HPP

#include <list>
#include <functional>

struct ImColor;

namespace GenericESP {
	class ESP {
	public:
		virtual std::function<void(const std::string&, bool&)> createBoolRenderer(const std::function<void()>& onChange = [] {}) = 0;
		virtual std::function<void(const std::string&, ImColor&)> createColorRenderer(const std::function<void()>& onChange = [] {}) = 0;
		virtual std::function<void(const std::string&, std::size_t&)> createComboRenderer(const std::list<std::string>& localization, const std::function<void()>& onChange = [] {}) = 0;
		virtual std::function<void(const std::string&, float&)> createFloatRenderer(float min, float max, const char* fmt, const std::function<void()>& onChange = [] {}) = 0;
		virtual std::function<void(const std::string&, int&)> createIntRenderer(int min, int max, const std::function<void()>& onChange = [] {}) = 0;
	};
}

#endif
