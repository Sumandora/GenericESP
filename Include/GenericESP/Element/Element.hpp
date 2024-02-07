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
		Element(Element&&) = delete; // Moving elements is likely to fail because Mixables use lambdas excessively. If you are in need of moving an element, use a unique ptr or something else that stores the element indirectly
		~Element() override;
	};

}

#endif
