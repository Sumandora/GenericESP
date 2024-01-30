#ifndef GENERICESP_MIXABLE_HPP
#define GENERICESP_MIXABLE_HPP

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
#include <memory>

#include "../Abstract/Renderable.hpp"
#include "../RendererFactory.hpp"
#include "../Serialization/Serialization.hpp"
#include "ConfigurableValue.hpp"
#include "imgui.h"

namespace GenericESP {
	struct MixableBase : Renderable, Serializable {
		std::string id; // Change this to break configs, if you want to rename settings then get the correct one from Mixable#options and call rename

		explicit MixableBase(std::string id) : id(std::move(id)) {}

		void serialize(SerializedTypeMap& parent) const {
			parent.putSubtree(id, serialize());
		}

		void deserializeFromParent(const SerializedTypeMap& parent) {
			auto opt = parent.getSubtree(id);
			if(opt.has_value())
				deserialize(opt.value());
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
		std::variant<ConfigurableType, MultiType> options; // When editing directly make sure to call rebuildRenderer

		using IsVisible = std::function<bool()>;
		IsVisible isVisible; // This can also be changed later

		ComboRenderer renderer;
		std::vector<std::string> ids;

		explicit Mixable(ConfigurableType t, IsVisible isVisible = [] { return true; })
			: MixableBase(t.getId())
			, options(std::move(t))
			, isVisible(std::move(isVisible))
		{
			rebuildRenderer();
		}

		explicit Mixable(std::string id, MultiType multiType, IsVisible isVisible = [] { return true; })
			: MixableBase(std::move(id))
			, options(std::move(multiType))
			, isVisible(std::move(isVisible))
		{
			rebuildRenderer();
		}

		explicit Mixable(std::string id, TypeList typeList, IsVisible isVisible = [] { return true; })
			: Mixable(id, MultiType{ 0, std::move(typeList) }, std::move(isVisible))
		{
		}

		void rebuildIds() {
			ids.clear();
			if(isMultiType()) {
				auto& multi = std::get<MultiType>(options);
				ids.reserve(multi.second.size());
				for (const ConfigurableType& type : multi.second) {
					ids.push_back(type.getId());
				}
			} else {
				auto& single = std::get<ConfigurableType>(options);
				ids.push_back(single.getId());
			}
		}

		void rebuildRenderer() {
			rebuildIds();
			renderer = rendererFactory.createComboRenderer(ids);
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
			rebuildRenderer();
		}

		void renderGui() override
		{
			if (!isVisible())
				return; // :/

			ImGui::PushID(id.c_str());

			if (isMultiType()) {
				auto& pair = std::get<MultiType>(options);

				renderer("Selector", pair.first);

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
				return map;
			}

			const ConfigurableType& configuredType = std::get<ConfigurableType>(options);
			return configuredType.serialize();
		}

		void deserialize(const SerializedTypeMap& map) override {
			if (isMultiType()) {
				auto& pair = std::get<MultiType>(options);
				auto indexOpt = map.get<std::size_t>("Selected Index");
				if(indexOpt.has_value())
					pair.first = indexOpt.value();

				for (ConfigurableType& type : pair.second) {
					auto typeOpt = map.getSubtree(type.getId());
					if(typeOpt.has_value())
						type.deserialize(typeOpt.value());
				}
				return;
			}
			ConfigurableType& configuredType = std::get<ConfigurableType>(options);
			configuredType.deserialize(map);
		}

		auto operator()(const EntityType* e) const
		{
			return this->getSelected().getConfigurable(e);
		}
	};

}

#endif
