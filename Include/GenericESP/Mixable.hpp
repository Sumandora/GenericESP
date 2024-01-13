#ifndef GENERICESP_MIXABLE_HPP
#define GENERICESP_MIXABLE_HPP

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "imgui.h"

namespace GenericESP {
	struct Renderable {
		virtual ~Renderable() = default;

		virtual void renderGui(const std::string& id) = 0;
	};

	template <typename Type>
		requires std::is_base_of_v<Renderable, Type>
	struct Mixable {
		using TypeTable = std::vector<std::pair<std::string, Type>>;
		using MultiType = std::pair<std::size_t /*selected index*/, TypeTable>;
		std::variant<Type, MultiType> options;

		using IsVisible = std::function<bool()>;
		IsVisible isVisible; // This can also be changed later

		std::string id; // Can be used to rename the thing

		Mixable(std::string&& id, Type&& t, IsVisible&& isVisible = [] { return true; })
			: id(std::move(id))
			, options(std::move(t))
			, isVisible(std::move(isVisible))
		{
		}

		Mixable(std::string&& id, MultiType&& t, IsVisible&& isVisible = [] { return true; })
			: id(std::move(id))
			, options(std::move(t))
			, isVisible(std::move(isVisible))
		{
		}

		Mixable(
			std::string&& id, TypeTable&& t, IsVisible&& isVisible = [] { return true; })
			: Mixable(
				std::move(id), MultiType{ 0, std::move(t) }, std::move(isVisible))
		{
		}

		[[nodiscard]] bool isSingleType() const
		{
			return std::holds_alternative<Type>(options);
		}

		[[nodiscard]] bool isMultiType() const
		{
			return std::holds_alternative<MultiType>(options);
		}

		[[nodiscard]] const Type& getSelected() const
		{
			if (isSingleType())
				return std::get<Type>(options);

			auto& pair = std::get<MultiType>(options);
			auto& selectedPair = *std::next(pair.second.begin(), pair.first);
			return selectedPair.second;
		}

		[[nodiscard]] Type& getSelected()
		{
			if (isSingleType())
				return std::get<Type>(options);

			auto& pair = std::get<MultiType>(options);
			auto& selectedPair = *std::next(pair.second.begin(), pair.first);
			return selectedPair.second;
		}

		void convertToMultiType(const std::string& specifier = "Default")
		{
			if (isMultiType())
				throw std::logic_error("Mixable is already multi type");

			auto& _default = std::get<Type>(options);
			options = MultiType{ 0, TypeTable{ { specifier, std::move(_default) } } };
		}

		void addType(std::string specifier, Type&& type)
		{
			if (isSingleType())
				convertToMultiType();

			auto& pair = std::get<MultiType>(options);
			pair.second.emplace_back(specifier, type);
		}

		void renderGui()
		{
			if (!isVisible())
				return; // :/

			if (isMultiType()) {
				auto& pair = std::get<MultiType>(options);

				const char* names[pair.second.size()];
				size_t idx = 0;
				for (const auto& [name, _] : pair.second) {
					names[idx] = name.c_str();
					idx++;
				}

				ImGui::PushID(id.c_str());
				int i = pair.first;
				ImGui::Combo(id.c_str(), &i, names, IM_ARRAYSIZE(names));
				pair.first = i;

				auto& selectedPair = *std::next(pair.second.begin(), pair.first);
				selectedPair.second.renderGui(id);

				ImGui::PopID();
				return;
			}
			std::get<Type>(options).renderGui(id);
		}
	};

}

#endif
