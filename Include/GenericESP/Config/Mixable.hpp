#ifndef GENERICESP_MIXABLE_HPP
#define GENERICESP_MIXABLE_HPP

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
#include <memory>

#include "ConfigurableValue.hpp"
#include "GenericESP/Serialization/Serialization.hpp"
#include "imgui.h"

#include "../Abstract/Renderable.hpp"

namespace GenericESP {
	struct MixableBase : Renderable, Serializable {
		std::string id; // Change this to break configs, if you want to rename settings then get the correct one from Mixable#options and call rename

		explicit MixableBase(std::string id) : id(std::move(id)) {}

		void serialize(SerializedTypeMap& parent) const {
			parent.putSubtree(id, serialize());
		}

		void deserializeFromParent(const SerializedTypeMap& parent) {
			deserialize(parent.getSubtree(id));
		}

		void renderGui() override = 0;
		[[nodiscard]] SerializedTypeMap serialize() const override = 0;
		void deserialize(const SerializedTypeMap &map) override = 0;
	};

	template <typename Type>
	struct Mixable : MixableBase {
		using ConfigurableType = ConfigurableValue<Type>;
		using TypeList = std::vector<ConfigurableType>;
		using MultiType = std::pair<std::size_t /*selected index*/, TypeList>;
		std::variant<ConfigurableType, MultiType> options;

		using IsVisible = std::function<bool()>;
		IsVisible isVisible; // This can also be changed later

		explicit Mixable(ConfigurableType t, IsVisible isVisible = [] { return true; })
			: MixableBase(t.getId())
			, options(std::move(t))
			, isVisible(std::move(isVisible))
		{
		}

		explicit Mixable(std::string id, MultiType multiType, IsVisible isVisible = [] { return true; })
			: MixableBase(std::move(id))
			, options(std::move(multiType))
			, isVisible(std::move(isVisible))
		{
		}

		explicit Mixable(std::string id, TypeList typeList, IsVisible isVisible = [] { return true; })
			: Mixable(id, MultiType{ 0, std::move(typeList) }, std::move(isVisible))
		{
		}

		[[nodiscard]] bool isSingleType() const
		{
			return std::holds_alternative<ConfigurableType>(options);
		}

		[[nodiscard]] bool isMultiType() const
		{
			return std::holds_alternative<MultiType>(options);
		}

		[[nodiscard]] const ConfigurableType& getSelected() const
		{
			if (isSingleType())
				return std::get<ConfigurableType>(options);

			auto& pair = std::get<MultiType>(options);
			auto& configuredValue = *std::next(pair.second.begin(), pair.first);
			return configuredValue;
		}

		[[nodiscard]] ConfigurableType& getSelected()
		{
			if (isSingleType())
				return std::get<ConfigurableType>(options);

			auto& pair = std::get<MultiType>(options);
			auto& configuredValue = *std::next(pair.second.begin(), pair.first);
			return configuredValue;
		}

		// Call this thing yourself when you want a custom name
		void convertToMultiType()
		{
			if (isMultiType())
				throw std::logic_error("Mixable is already multi type");

			auto& _default = std::get<ConfigurableType>(options);
			options = MultiType{ 0, TypeList{ { std::move(_default) } } };
		}

		void addType(ConfigurableType type)
		{
			if (isSingleType())
				convertToMultiType();

			auto& pair = std::get<MultiType>(options);
			pair.second.emplace_back(type);
		}

		void renderGui() override
		{
			if (!isVisible())
				return; // :/

			ImGui::PushID(id.c_str());

			if (isMultiType()) {
				auto& pair = std::get<MultiType>(options);

				const char* names[pair.second.size()];
				size_t idx = 0;
				for (const ConfigurableType& type : pair.second) {
					names[idx] = type.getId().c_str();
					idx++;
				}

				int i = pair.first;
				ImGui::Combo("Selector", &i, names, IM_ARRAYSIZE(names));
				pair.first = i;

				ImGui::PushID("Mixable types");
				ConfigurableType& configuredType = *std::next(pair.second.begin(), pair.first);
				configuredType.renderGui();
				ImGui::PopID();
			} else {
				ConfigurableType& configuredType = std::get<ConfigurableType>(options);
				configuredType.renderGui();
			}

			ImGui::PopID();
		}

		[[nodiscard]] SerializedTypeMap serialize() const override {
			SerializedTypeMap map;

			if (isMultiType()) {
				auto& pair = std::get<MultiType>(options);
				map["Selected Index"] = pair.first;

				for (const ConfigurableType& type : pair.second) {
					map.putSubtree(type.getId(), type.serialize());
				}
			} else {
				const ConfigurableType& configuredType = std::get<ConfigurableType>(options);
				map.putSubtree(configuredType.getId(), configuredType.serialize());
			}
			return map;
		}

		void deserialize(const SerializedTypeMap& map) override {
			if (isMultiType()) {
				auto& pair = std::get<MultiType>(options);
				pair.first = map.get<std::size_t>("Selected Index");

				for (ConfigurableType& type : pair.second) {
					type.deserialize(map.getSubtree(type.getId()));
				}
			} else {
				ConfigurableType& configuredType = std::get<ConfigurableType>(options);
				configuredType.deserialize(map.getSubtree(configuredType.getId()));
			}
		}

		auto operator()(const EntityType* e) const
		{
			return this->getSelected().getConfigurable(e);
		}
	};

}

#endif