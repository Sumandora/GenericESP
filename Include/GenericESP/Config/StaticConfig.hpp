#ifndef GENERICESP_CONFIG_STATICCONFIG_HPP
#define GENERICESP_CONFIG_STATICCONFIG_HPP

#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <concepts>

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

		template <std::size_t Idx = std::variant_size_v<SerializedType> - 1>
		consteval static bool canUseDefaultSerializers() {
			bool result = std::is_same_v<Configurable, std::variant_alternative_t<Idx, SerializedType>>;
			if constexpr (Idx > 0)
				return result || canUseDefaultSerializers<Idx - 1>();
			return result;
		}

		template <typename = std::enable_if<canUseDefaultSerializers()>>
		StaticConfig(std::string id,
			Configurable thing,
			decltype(renderer) renderer)
			: id(std::move(id))
			, thing(std::move(thing))
			, renderer(std::move(renderer))
			, serializer([](const Configurable& thing) {
				SerializedTypeMap map;
				map["Value"] = thing;
				return map;
			})
			, deserializer([](const SerializedTypeMap& map, Configurable& thing) {
				auto opt = map.get<Configurable>("Value");
				if(opt.has_value())
					thing = opt.value();
			})
		{
		}

		StaticConfig(std::string id,
			Configurable thing,
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
