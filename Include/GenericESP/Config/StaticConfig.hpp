#ifndef GENERICESP_CONFIG_STATICCONFIG_HPP
#define GENERICESP_CONFIG_STATICCONFIG_HPP

#include <functional>
#include <string>

namespace GenericESP {
	template <typename Configurable>
	struct StaticConfig : Serializable {
		Configurable thing;
		using RenderFunc = std::function<void(const std::string&, Configurable&)>;
		RenderFunc render;

		StaticConfig(Configurable c, RenderFunc f) : thing(std::move(c)), render(std::move(f)) {}

		static constexpr bool isSerializable = std::is_base_of<Serializable, Configurable>::value;

		[[nodiscard]] Serialization serialize() const override {
			if constexpr (isSerializable)
				return thing.serialize();
			else
				return Serialization("Value", thing);
		}

		void deserialize(const GenericESP::Serialization & data) override {
			if constexpr (isSerializable)
				thing.deserialize(data);
			else
				thing = data.get<decltype(thing)>("Value").value();
		}
	};
}

#endif
