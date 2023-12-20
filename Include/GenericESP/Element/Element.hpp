#ifndef GENERICESP_ELEMENT_HPP
#define GENERICESP_ELEMENT_HPP

#include "../Config/MixableConfigurationValue.hpp"
#include "../Renderer/BoolRenderer.hpp"

#include "GenericESP/BaseClasses/Serializable.hpp"

namespace GenericESP {

	template <typename EntityType>
	struct Element : Serializable {
		MixableConfigurableValue<bool, EntityType> enabled{
			"Enabled",
			StaticConfig<bool>{ false, createBoolRenderer() }
		};

		[[nodiscard]] Serialization serialize() const override = 0;
		void deserialize(const Serialization&) override = 0;
	};

}

#endif
