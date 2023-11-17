#ifndef GENERICESP_CONFIG_STATICCONFIG_HPP
#define GENERICESP_CONFIG_STATICCONFIG_HPP

#include <functional>
#include <string>

namespace GenericESP {
	template <typename Configurable>
	struct StaticConfig {
		Configurable thing;
		std::function<void(const std::string&, Configurable&)> render;
	};
}

#endif
