#ifndef GENERICESP_CONFIG_DYNAMICCONFIG_HPP
#define GENERICESP_CONFIG_DYNAMICCONFIG_HPP

#include <functional>
#include <string>

namespace GenericESP {
	template <typename Configurable, typename EntityType>
	struct DynamicConfig : Serializable {
		std::function<Configurable(const EntityType&)> thing;
		std::function<void(const std::string&)> render;

		DynamicConfig(std::function<Configurable(const EntityType&)>&& thing, std::function<void(const std::string&)>&& render) : thing(std::move(thing)), render(std::move(render)) {}

		[[nodiscard]] Serialization serialize() const override {
			return {};
		}
		void deserialize(const Serialization&) override {}
	};
}

#endif
