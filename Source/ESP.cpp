#include "GenericESP/ESP.hpp"

#include "GenericESP/Element/Element.hpp"

#include "imgui.h"

#include <algorithm>

using namespace GenericESP;

bool ESP::isDefinitelyDisabled() const {
	return std::ranges::all_of(elements, [](const Element* element) {
		auto selected = element->enabled.getSelected();
		return selected.isStatic() && !selected.getStaticConfig().thing;
	});
}

bool ESP::isEnabled(const EntityType* e) const {
	return std::ranges::any_of(elements, [e](const Element* element) { return element->enabled.getSelected().getConfigurable(e); });
}

void ESP::renderGui()
{
	if (ImGui::BeginTabBar("Elements", ImGuiTabBarFlags_Reorderable)) {
		for (Element* e : elements)
			if (ImGui::BeginTabItem(e->id.c_str())) {
				e->renderGui();
				ImGui::EndTabItem();
			}
		ImGui::EndTabBar();
	}
}

[[nodiscard]] SerializedTypeMap ESP::serialize() const
{
	SerializedTypeMap map;
	for (Element* e : elements)
		map.putSubtree(e->id, e->serialize());
	return map;
}

void ESP::deserialize(const SerializedTypeMap& map)
{
	for (Element* e : elements) {
		auto opt = map.getSubtree(e->id);
		if(opt.has_value())
			e->deserialize(opt.value());
	}
}
