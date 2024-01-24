#ifndef GENERICESP_CONFIG_CONFIGURABLEVALUE_HPP
#define GENERICESP_CONFIG_CONFIGURABLEVALUE_HPP

#include "DynamicConfig.hpp"
#include "StaticConfig.hpp"

namespace GenericESP {

	template <typename Configurable>
	struct ConfigurableValue final : Renderable, Serializable {
		~ConfigurableValue() override = default;

		std::variant<StaticConfig<Configurable>, DynamicConfig<Configurable>> thing;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
		ConfigurableValue(StaticConfig<Configurable> s)
			: thing(std::move(s))
		{
		}

		ConfigurableValue(DynamicConfig<Configurable> d)
			: thing(std::move(d))
		{
		}
#pragma clang diagnostic pop

		[[nodiscard]] bool isStatic() const
		{
			return std::holds_alternative<StaticConfig<Configurable>>(thing);
		}

		[[nodiscard]] const StaticConfig<Configurable>& getStaticConfig() const
		{
			return std::get<StaticConfig<Configurable>>(thing);
		}

		[[nodiscard]] const DynamicConfig<Configurable>& getDynamicConfig() const
		{
			return std::get<DynamicConfig<Configurable>>(thing);
		}

		StaticConfig<Configurable>& getStaticConfig()
		{
			return std::get<StaticConfig<Configurable>>(thing);
		}

		DynamicConfig<Configurable>& getDynamicConfig()
		{
			return std::get<DynamicConfig<Configurable>>(thing);
		}

		[[nodiscard]] const std::string& getId() const {
			if (isStatic())
				return getStaticConfig().id;
			return getDynamicConfig().id;
		}

		void rename(std::string newId) {
			if (isStatic())
				getStaticConfig().id = newId;
			else
				getDynamicConfig().id = newId;
		}

		[[nodiscard]] Configurable getConfigurable(const EntityType* e) const
		{
			if (isStatic())
				return getStaticConfig().thing;
			return getDynamicConfig().thing(e);
		}

		Configurable getConfigurable(const EntityType* e)
		{
			if (isStatic())
				return getStaticConfig().thing;
			return getDynamicConfig().thing(e);
		}

		void renderGui() override
		{
			if (isStatic()) {
				auto& staticConfig = getStaticConfig();
				return staticConfig.renderGui();
			}
			return getDynamicConfig().renderGui();
		}

		[[nodiscard]] SerializedTypeMap serialize() const override {
			if (isStatic()) {
				auto& staticConfig = getStaticConfig();
				return staticConfig.serialize();
			}
			return getDynamicConfig().serialize();
		}

		void deserialize(const SerializedTypeMap& map) override {
			if (isStatic()) {
				auto& staticConfig = getStaticConfig();
				return staticConfig.deserialize(map);
			}
			return getDynamicConfig().deserialize(map);
		}
	};
}

#endif
