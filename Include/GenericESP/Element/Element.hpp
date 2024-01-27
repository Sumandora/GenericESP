#ifndef GENERICESP_ELEMENT_HPP
#define GENERICESP_ELEMENT_HPP

#include "../Config/Mixable.hpp"
#include "GenericESP/ESP.hpp"

namespace GenericESP {

	struct Element : Renderable, Serializable {
		ESP* base;
		std::string id;
		bool topLevel;

		Mixable<bool> enabled;

		explicit Element(ESP* base, std::string id, bool topLevel);
		~Element() override;
	};

}

#endif
