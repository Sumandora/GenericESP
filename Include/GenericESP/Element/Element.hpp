#ifndef GENERICESP_ELEMENT_HPP
#define GENERICESP_ELEMENT_HPP

#include "../Config/MixableConfigurationValue.hpp"
#include "GenericESP/ESP.hpp"

namespace GenericESP {

	struct Element {
		MixableConfigurableValue<bool> enabled;

		explicit Element(ESP* base);
	};

}

#endif
