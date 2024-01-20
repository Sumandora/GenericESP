#ifndef GENERICESP_MIXABLE_HPP
#define GENERICESP_MIXABLE_HPP

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "imgui.h"
#include "Serialization/Serialization.hpp"

namespace GenericESP {
	struct Renderable {
		virtual ~Renderable() = default;

		virtual void renderGui(const std::string& id) = 0;
	};

	template <typename Type>
		requires std::conjunction_v<std::is_base_of<Renderable, Type>, std::is_base_of<Serializable, Type>>
	struct Mixable : Serializable {
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

		Mixable(std::string&& id, TypeTable&& t, IsVisible&& isVisible = [] { return true; })
			: Mixable(std::move(id), MultiType{ 0, std::move(t) }, std::move(isVisible))
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

				int i = pair.first;
				ImGui::Combo(id.c_str(), &i, names, IM_ARRAYSIZE(names));
				pair.first = i;

				ImGui::PushID(id.c_str());
				auto& selectedPair = *std::next(pair.second.begin(), pair.first);
				selectedPair.second.renderGui(id);
				ImGui::PopID();
			} else
				std::get<Type>(options).renderGui(id);
		}

		SerializedTypeMap serialize() const override {
			SerializedTypeMap map;

			if (isMultiType()) {
				auto& pair = std::get<MultiType>(options);
				map["Selected Index"] = pair.first;

				for (const auto& [name, type] : pair.second) {
					map[name] = type.serialize();
				}
			} else
				map[id] = std::get<Type>(options).serialize();
			return map;
		}

		void deserialize(const SerializedTypeMap& map) override {
			if (isMultiType()) {
				auto& pair = std::get<MultiType>(options);
				pair.first = std::get<std::size_t>(map.at("Selected Index"));

				for (auto& [name, type] : pair.second) {
					type.deserialize(map.at(name));
				}
			} else
				std::get<Type>(options).deserialize(map.at(id));
		}
	};

}

#endif
