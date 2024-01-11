#ifndef GENERICESP_ELEMENT_HPP
#define GENERICESP_ELEMENT_HPP

#include "../Config/MixableConfigurationValue.hpp"
#include "../Renderer/BoolRenderer.hpp"

namespace GenericESP {

	struct Element {
		MixableConfigurableValue<bool> enabled{
			"Enabled",
			StaticConfig<bool>{ false, createBoolRenderer() }
		};
	};

}

#endif
