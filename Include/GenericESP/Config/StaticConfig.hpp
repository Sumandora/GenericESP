#ifndef GENERICESP_CONFIG_STATICCONFIG_HPP
#define GENERICESP_CONFIG_STATICCONFIG_HPP

#include <functional>
#include <string>
#include <optional>

#include "../Serialization/Serialization.hpp"

namespace GenericESP {
	template <typename Configurable>
	struct StaticConfig : Serializable {
		Configurable thing;
		std::function<void(const std::string&, Configurable&)> render;
		std::function<SerializedTypeMap()> serializer;
		std::function<void(const SerializedTypeMap&)> deserializer;

		StaticConfig(Configurable&& thing, decltype(render)&& render, std::optional<std::function<SerializedTypeMap()>> serializer = std::nullopt, std::optional<std::function<void(const SerializedTypeMap&)>> deserializer = std::nullopt)
			: thing(std::move(thing))
			, render(std::move(render))
			, serializer(serializer.has_value() ? serializer.value() : [&] {
				SerializedTypeMap map;
				map["Value"] = thing;
				return map;
			})
			, deserializer(deserializer.has_value() ? deserializer.value() : [&](const SerializedTypeMap& t) { thing = std::get<Configurable>(t.at("Value")); })
		{
		}

		SerializedTypeMap serialize() const override
		{
			return serializer();
		}

		void deserialize(const SerializedTypeMap& t) override
		{
			return deserializer(t);
		}
	};
}

#endif
