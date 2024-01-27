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

		std::vector<std::unique_ptr<Flag>> flags;
		VectorOrdering<std::unique_ptr<Flag>> flagOrder;

		explicit Flags(ESP* base, std::string id, std::initializer_list<Flag*> flags, bool topLevel = true);

		// --- Likely irrelevant for users ---
		ImVec2 drawEntry(ImDrawList* drawList, const EntityType* e, ImRect& rect, const std::unique_ptr<Flag>& flag, float yOffset) const;
		// --- Likely irrelevant for users ---

		void draw(ImDrawList* drawList, const EntityType* e, UnionedRect& unionedRect) const;
		void renderGui() override;
		[[nodiscard]] SerializedTypeMap serialize() const override;
		void deserialize(const SerializedTypeMap &map) override;
	};

}

#endif
