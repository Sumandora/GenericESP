#ifndef GENERICESP_CONFIG_DYNAMICCONFIG_HPP
#define GENERICESP_CONFIG_DYNAMICCONFIG_HPP

#include <functional>
#include <string>

namespace GenericESP {
	template <typename Configurable, typename EntityType>
	struct DynamicConfig {
		std::function<Configurable(const EntityType&)> thing;
		std::function<void(const std::string&)> render;
	};
}

#endif
