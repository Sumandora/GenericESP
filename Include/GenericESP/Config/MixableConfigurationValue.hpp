#ifndef GENERICESP_CONFIG_MIXABLECONFIGURATIONVALUE_HPP
#define GENERICESP_CONFIG_MIXABLECONFIGURATIONVALUE_HPP

#include "ConfigurableValue.hpp"

namespace GenericESP {
	template <typename Configurable>
	class MixableConfigurableValue : public Mixable<ConfigurableValue<Configurable>> {
	public:
		using Mixable<ConfigurableValue<Configurable>>::Mixable;

		auto operator()(const void* e) const
		{
			return this->getSelected().getConfigurable(e);
		}
	};
}
#endif
