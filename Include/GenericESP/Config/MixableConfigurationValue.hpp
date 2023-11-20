#ifndef GENERICESP_CONFIG_MIXABLECONFIGURATIONVALUE_HPP
#define GENERICESP_CONFIG_MIXABLECONFIGURATIONVALUE_HPP

#include "ConfigurableValue.hpp"

namespace GenericESP {
	template <typename Configurable, typename EntityType>
	struct MixableConfigurableValue : Mixable<ConfigurableValue<Configurable, EntityType>> {
		using Mixable<ConfigurableValue<Configurable, EntityType>>::Mixable;

		auto operator()(const EntityType& e) const
		{
			return this->getSelected().getConfigurable(e);
		}
	};
}
#endif