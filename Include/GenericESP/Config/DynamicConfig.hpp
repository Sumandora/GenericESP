#ifndef GENERICESP_CONFIG_DYNAMICCONFIG_HPP
#define GENERICESP_CONFIG_DYNAMICCONFIG_HPP

#include <functional>
#include <string>

#include "../ESP.hpp"
#include "../Serialization/Serialization.hpp"

namespace GenericESP {
	template <typename Configurable>
	struct DynamicConfig : Renderable, Serializable {
		std::function<Configurable(const EntityType*)> thing;
		std::function<void(const std::string&)> render;
		std::function<SerializedTypeMap()> serializer;
		std::function<void(const SerializedTypeMap&)> deserializer;

		DynamicConfig(
			std::function<Configurable(const EntityType*)>&& thing,
			std::function<void(const std::string&)>&& render,
			std::function<SerializedTypeMap()>&& serializer,
			std::function<void(const SerializedTypeMap&)>&& deserializer) : thing(std::move(thing)), render(std::move(render)), serializer(std::move(serializer)), deserializer(std::move(deserializer)){

		}

		void renderGui(const std::string &id) override {
			return render(id);
		}

		SerializedTypeMap serialize() const override {
			return serializer();
		}

		void deserialize(const SerializedTypeMap& t) override {
			return deserializer(t);
		}
	};
}

#endif
