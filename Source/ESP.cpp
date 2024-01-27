#include "GenericESP/ESP.hpp"

#include "GenericESP/Element/Element.hpp"

#include "imgui.h"

using namespace GenericESP;

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
	for (const Element* e : elements)
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