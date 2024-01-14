#ifndef GENERICESP_CONFIG_DYNAMICCONFIG_HPP
#define GENERICESP_CONFIG_DYNAMICCONFIG_HPP

#include <functional>
#include <string>

#include "../ESP.hpp"

namespace GenericESP {
	template <typename Configurable>
	struct DynamicConfig {
		std::function<Configurable(const EntityType*)> thing;
		std::function<void(const std::string&)> render;
	};
}

#endif
