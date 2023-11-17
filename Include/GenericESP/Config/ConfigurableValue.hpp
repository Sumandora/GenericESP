#ifndef GENERICESP_CONFIG_CONFIGURABLEVALUE_HPP
#define GENERICESP_CONFIG_CONFIGURABLEVALUE_HPP

#include "../Mixable.hpp"
#include "DynamicConfig.hpp"
#include "StaticConfig.hpp"

namespace GenericESP {

	template <typename Configurable, typename EntityType>
	struct ConfigurableValue final : Renderable {
		~ConfigurableValue() override = default;

		std::variant<StaticConfig<Configurable>, DynamicConfig<Configurable, EntityType>> thing;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
		ConfigurableValue(StaticConfig<Configurable> s)
			: thing(s)
		{
		}

		ConfigurableValue(DynamicConfig<Configurable, EntityType> d)
			: thing(d)
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

		[[nodiscard]] const DynamicConfig<Configurable, EntityType>& getDynamicConfig() const
		{
			return std::get<DynamicConfig<Configurable, EntityType>>(thing);
		}

		StaticConfig<Configurable>& getStaticConfig()
		{
			return std::get<StaticConfig<Configurable>>(thing);
		}

		DynamicConfig<Configurable, EntityType>& getDynamicConfig()
		{
			return std::get<DynamicConfig<Configurable, EntityType>>(thing);
		}

		[[nodiscard]] Configurable getConfigurable(const EntityType& e) const
		{
			if (isStatic())
				return getStaticConfig().thing;
			return getDynamicConfig().thing(e);
		}

		Configurable getConfigurable(const EntityType& e)
		{
			if (isStatic())
				return getStaticConfig().thing;
			return getDynamicConfig().thing(e);
		}

		void renderGui(const std::string& id) override
		{
			if (isStatic()) {
				auto& staticConfig = getStaticConfig();
				return staticConfig.render(id, staticConfig.thing);
			}
			getDynamicConfig().render(id);
		}
	};
}

#endif
