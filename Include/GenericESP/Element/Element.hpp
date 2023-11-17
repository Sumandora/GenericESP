#ifndef GENERICESP_ELEMENT_HPP
#define GENERICESP_ELEMENT_HPP

#include "../Config/MixableConfigurationValue.hpp"
#include "../Renderer/BoolRenderer.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct Element {
		MixableConfigurableValue<bool, EntityType> enabled{
			"Enabled",
			StaticConfig<bool>{ false, createBoolRenderer() }
		};
	};

}

#endif
