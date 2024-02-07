#ifndef GENERICESP_ELEMENT_FLAGS_HPP
#define GENERICESP_ELEMENT_FLAGS_HPP

#include "../Flag.hpp"
#include "../VectorOrdering.hpp"
#include "SidedElement.hpp"

#include <memory>
#include <ranges>

namespace GenericESP {

	struct Flags : SidedElement {
		Mixable<float> spacing;
		Mixable<float> lineSpacing;

		std::vector<Flag*> flags;
		VectorOrdering<Flag*> flagOrder;

		// All pointers in 'flags' will be taken ownership of.
		explicit Flags(ESP* base, std::string id, std::initializer_list<Flag*> flags, bool topLevel = true);
		~Flags() override;

		// --- Likely irrelevant for users ---
		ImVec2 drawEntry(ImDrawList* drawList, const EntityType* e, ImRect& rect, const Flag* flag, float yOffset) const;
		// --- Likely irrelevant for users ---

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const;
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap &map) override;
	};

}

#endif
