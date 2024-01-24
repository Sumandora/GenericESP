#ifndef GENERICESP_ABSTRACT_RENDERABLE_HPP
#define GENERICESP_ABSTRACT_RENDERABLE_HPP

namespace GenericESP {

	struct Renderable {
		virtual ~Renderable() = default;

		virtual void renderGui() = 0;
	};

}

#endif
