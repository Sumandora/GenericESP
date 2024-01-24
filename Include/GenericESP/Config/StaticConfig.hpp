#ifndef GENERICESP_CONFIG_STATICCONFIG_HPP
#define GENERICESP_CONFIG_STATICCONFIG_HPP

#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

#include "../Abstract/Renderable.hpp"
#include "../Abstract/Serializable.hpp"

#include "imgui.h"

namespace GenericESP {
	template <typename Configurable>
	struct StaticConfig : Renderable, Serializable {
		std::string id;
		Configurable thing;
		std::function<void(const std::string&, Configurable&)> renderer;
		std::function<SerializedTypeMap(const Configurable&)> serializer;
		std::function<void(const SerializedTypeMap&, Configurable&)> deserializer;

		StaticConfig(std::string id,
			Configurable thing,
			decltype(renderer) renderer,
			std::optional<std::function<SerializedTypeMap(const Configurable&)>> serializer = std::nullopt,
			std::optional<std::function<void(const SerializedTypeMap&, Configurable&)>> deserializer = std::nullopt)
			: id(std::move(id))
			, thing(std::move(thing))
			, renderer(std::move(renderer))
			, serializer(serializer.has_value() ? serializer.value() : [](const Configurable& thing) {
				SerializedTypeMap map;
				if constexpr (std::is_base_of_v<ImColor, Configurable>) {
					// TODO
					map["X"] = thing.Value.x;
					map["Y"] = thing.Value.y;
					map["Z"] = thing.Value.z;
					map["W"] = thing.Value.w;
				} else {
					map["Value"] = thing;
				}
				return map;
			})
			, deserializer(deserializer.has_value() ? deserializer.value() : [](const SerializedTypeMap& map, Configurable& thing) {
				if constexpr (std::is_base_of_v<ImColor, Configurable>) {
					// TODO
					thing = ImColor{
						map.get<float>("X"),
						map.get<float>("Y"),
						map.get<float>("Z"),
						map.get<float>("W")
					};
				} else {
					thing = map.get<Configurable>("Value");
				}
			})
		{
		}

		void renderGui() override
		{
			return renderer(id, thing);
		}

		[[nodiscard]] SerializedTypeMap serialize() const override
		{
			return serializer(thing);
		}

		void deserialize(const SerializedTypeMap& map) override
		{
			return deserializer(map, thing);
		}
	};
}

#endif
