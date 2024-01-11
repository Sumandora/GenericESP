#ifndef GENERICESP_HPP
#define GENERICESP_HPP

#include "GenericESP/Element/Rectangle.hpp"
#include "GenericESP/Element/Bar.hpp"
#include "GenericESP/Element/Text.hpp"
#include "GenericESP/Element/SidedText.hpp"
#include "GenericESP/Element/Circle.hpp"
#include "GenericESP/Element/Line.hpp"
#include "GenericESP/Element/Flags.hpp"

namespace GenericESP {

	template<typename T, typename R = const void*>
	std::function<R(const void*)> lambda(const std::function<R(const T&)>& func) {
		return [func](const void* ent) {
			return func(*reinterpret_cast<const T*>(ent));
		};
	}

}

#endif
