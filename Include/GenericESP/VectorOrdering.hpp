#ifndef GENERICESP_VECTORORDERING_HPP
#define GENERICESP_VECTORORDERING_HPP

#include "imgui.h"

#include <functional>
#include <string>
#include <vector>

namespace GenericESP {
	template <typename T>
	struct VectorOrdering {

		std::vector<T>& ref;
		using NameProvider = std::function<std::string(const T&)>;
		NameProvider nameProvider;

		VectorOrdering(std::vector<T>& ref, NameProvider&& nameProvider)
			: ref(ref)
			, nameProvider(std::move(nameProvider))
		{
		}

		int currentIdx = 0;
		void renderGui(const std::string& id)
		{
			ImGui::PushID(id.c_str());
			if (ImGui::BeginListBox("##Order")) {
				for (int i = 0; i < ref.size(); i++) {
					const bool selected = (currentIdx == i);
					if (ImGui::Selectable(nameProvider(ref.at(i)).c_str(), selected))
						currentIdx = i;

					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}
			ImGui::SameLine();
			ImGui::BeginDisabled(currentIdx == 0);
			if (ImGui::Button("Up")) {
				std::iter_swap(std::next(ref.begin(), currentIdx), std::next(ref.begin(), currentIdx - 1));
				currentIdx--;
			}
			ImGui::EndDisabled();
			ImGui::SameLine();
			ImGui::BeginDisabled(currentIdx == ref.size() - 1);
			if (ImGui::Button("Down")) {
				std::iter_swap(std::next(ref.begin(), currentIdx), std::next(ref.begin(), currentIdx + 1));
				currentIdx++;
			}
			ImGui::EndDisabled();

			ImGui::PopID();
		}
	};
}

#endif
