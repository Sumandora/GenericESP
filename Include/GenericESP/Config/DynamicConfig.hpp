#ifndef GENERICESP_CONFIG_DYNAMICCONFIG_HPP
#define GENERICESP_CONFIG_DYNAMICCONFIG_HPP

#include <functional>
#include <string>

#include "../ESP.hpp"

#include "../Abstract/Renderable.hpp"
#include "../Abstract/Serializable.hpp"

#include "../OpaqueLambda.hpp"

namespace GenericESP {
	template <typename Configurable>
	struct DynamicConfig : Renderable, Serializable {
		std::string id;

		OpaqueLambda<Configurable> thing;
		std::function<void(const std::string&)> renderer;
		std::function<SerializedTypeMap()> serializer;
		std::function<void(const SerializedTypeMap&)> deserializer;

		DynamicConfig(
			std::string id,
			decltype(thing) thing,
			decltype(renderer) renderer,
			decltype(serializer) serializer,
			decltype(deserializer) deserializer)
			: id(std::move(id))
			, thing(std::move(thing))
			, renderer(std::move(renderer))
			, serializer(std::move(serializer))
			, deserializer(std::move(deserializer))
		{
		}

		void renderGui() override
		{
			return renderer(id);
		}

		[[nodiscard]] SerializedTypeMap serialize() const override
		{
			return serializer();
		}

		void deserialize(const SerializedTypeMap& map) override
		{
			return deserializer(map);
		}
	};
}

#endif
