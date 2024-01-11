#ifndef GENERICESP_CONFIG_DYNAMICCONFIG_HPP
#define GENERICESP_CONFIG_DYNAMICCONFIG_HPP

#include <functional>
#include <string>

namespace GenericESP {
	template <typename Configurable>
	struct DynamicConfig {
		std::function<Configurable(const void*)> thing;
		std::function<void(const std::string&)> render;
	};
}

#endif
