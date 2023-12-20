#ifndef GENERICESP_BASECLASSES_RENDERABLE_HPP
#define GENERICESP_BASECLASSES_RENDERABLE_HPP

#include <string>

namespace GenericESP {
	struct Renderable {
		virtual ~Renderable() = default;

		virtual void renderGui(const std::string& id) = 0;
	};
}

#endif
