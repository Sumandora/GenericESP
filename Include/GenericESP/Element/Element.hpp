#ifndef GENERICESP_ELEMENT_HPP
#define GENERICESP_ELEMENT_HPP

#include "../Config/Mixable.hpp"
#include "GenericESP/ESP.hpp"

namespace GenericESP {

	struct Element : Renderable, Serializable {
		std::string id;

		Mixable<bool> enabled;

		explicit Element(ESP* base, std::string id);
	};

}

#endif
