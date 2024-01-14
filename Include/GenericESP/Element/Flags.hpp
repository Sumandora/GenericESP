#ifndef GENERICESP_ELEMENT_FLAGS_HPP
#define GENERICESP_ELEMENT_FLAGS_HPP

#include "../Flag.hpp"
#include "../VectorOrdering.hpp"
#include "SidedElement.hpp"

#include <memory>
#include <ranges>

namespace GenericESP {

	struct Flags : SidedElement {
		MixableConfigurableValue<float> spacing;

		std::vector<std::unique_ptr<Flag>> flags;
		VectorOrdering<std::unique_ptr<Flag>> flagOrder;

		explicit Flags(ESP* base, std::initializer_list<Flag*> flags);

		ImVec2 drawEntry(ImDrawList* drawList, const EntityType* e, ImRect& rect, const std::unique_ptr<Flag>& flag, float yOffset) const;
		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const;
		void renderGui(const std::string& id);
	};

}

#endif
